#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <PubSubClient.h>
#include <ModbusRTU.h>
#include "web_config.hpp"
#include "util.hpp"

#include "logging.hpp"
//#include "config.h"
#include "energy_meter_config.hpp"
#include "meters/sdm72d-m-v1.hpp"
#include "meters/sdm72d-m-v2.hpp"
#include "meters/dts238-7.hpp"
#include "meters/sdm630-v2.hpp"

std::shared_ptr<WiFiClient> wifiClient;
std::shared_ptr<PubSubClient> client;
String hostname = "energy-meter-rs485";

ModbusRTU mb1;

// D5 is connected to RXD on the RS485 board 
// and D6 ist connected to TXD
SoftwareSerial softSerial(D5, D6); // (RX, TX)

EnergyMeterConfig* energy_meter;
uint16_t data_per_meter;
uint16_t* buffer;
bool mqtt_enabled = true;

unsigned long last_update_time = 0;
String deviceId;
String topic;
bool led_state = false;

// Config Values
String mqtt_broker;
String mqtt_user;
String mqtt_password;
uint16_t mqtt_port;
EnergyMeterType meter_type;
uint8_t modbus_id;
uint8_t number_of_meters;
uint16_t baudrate;

void setup_wifi() {
  deviceId = mac_string();
  topic = String("energy-" + deviceId + "/");
  hostname = String("energy-" + deviceId);
  Serial.println("Device ID: ");
  Serial.println(deviceId);

  /*
  WiFi.mode(WIFI_STA);
  WiFi.hostname(hostname);
  WiFi.begin(config_wifi_ssid(), config_wifi_pw());
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  */
}

float get_float_reversed(uint16_t* data) {
  uint16_t swap[2];
  swap[0] = data[1];
  swap[1] = data[0];

  auto float_ptr = reinterpret_cast<float*>(swap);
  return *float_ptr;
}

float get_float(uint16_t* data) {
  auto float_ptr = reinterpret_cast<float*>(data);
  return *float_ptr;
}

constexpr uint32_t get_uint32_t(uint16_t* data) {
  return ((uint32_t)data[0] << 16) + (uint32_t)data[1];
}
constexpr int32_t get_int32_t(uint16_t* data) {
  return (int32_t)get_uint32_t(data);
}
constexpr uint16_t get_uint16_t(uint16_t* data) {
  return data[0];
}
constexpr int16_t get_int16_t(uint16_t* data) {
  return (int16_t)data[0];
}
constexpr uint8_t get_uint8_t_high_byte(uint16_t* data) {
  return (data[0] & (uint16_t)0xFF00) >> 8;
}
constexpr uint8_t get_uint8_t_low_byte(uint16_t* data) {
  return data[0] & (uint16_t)0x00FF;
}

void wait_for_result() {
    while(mb1.slave()) {
      mb1.task();
    }
}

void setup() {
  Serial.begin(74880);
  web_config_setup();
  pinMode(LED_BUILTIN, OUTPUT);

  mqtt_broker = config_mqtt_broker();
  mqtt_user = config_mqtt_user();
  mqtt_password = config_mqtt_password();
  mqtt_port = config_mqtt_port();

  meter_type = config_meter_type();
  modbus_id = config_modbus_id();
  number_of_meters = config_number_of_meters();
  baudrate = config_baudrate();

  Serial.println("Wifi SSID: " + config_wifi_ssid());
  Serial.println("Wifi Password: " + config_wifi_pw());
  Serial.println();
  Serial.println("MQTT Broker: " + mqtt_broker);
  Serial.println("MQTT User: " +   mqtt_user);
  Serial.println("MQTT Password: " + mqtt_password);
  Serial.println("MQTT Port: " + String(mqtt_port));
  Serial.println();
  Serial.println("Meter Type: " + meter_type_to_string(meter_type));
  Serial.println("Modbus ID: " + String(modbus_id));
  Serial.println("Number of Meters: " + String(number_of_meters));
  Serial.println("Baudrate: " + String(baudrate));
  Serial.println();

  setup_wifi();

  wifiClient = std::make_shared<WiFiClient>();
  client = std::make_shared<PubSubClient>(mqtt_broker.c_str(), mqtt_port, *wifiClient);

  // Setup modbus
  softSerial.begin(baudrate, SWSERIAL_8N1);
  mb1.begin(&softSerial);
  mb1.master();

  // Setup Configuration
  if(meter_type == EnergyMeterType::SDM72D_M_V2) {
    energy_meter = new Sdm72dmv2();
  } 
  else if (meter_type == EnergyMeterType::SDM72D_M_V1) {
    energy_meter = new Sdm72dmv1();
  } 
  else if (meter_type == EnergyMeterType::DTS238_7) {
    energy_meter = new Dts238_7();
  }
  else if (meter_type == EnergyMeterType::SDM630_V2) {
    energy_meter = new Sdm630v2();
  }
  else {
    Serial.println("Unknown Configuration, defaulting to SDM72D_M_V1");
    energy_meter = new Sdm72dmv1();
  }

  energy_meter->setup();

  Serial.println("Allocate Buffer");
  
  data_per_meter = energy_meter->data_per_meter;
  size_t buffer_size = data_per_meter*number_of_meters;
  buffer = new uint16_t[buffer_size];

  // Zero out data
  for (size_t i = 0; i < buffer_size; i++) {
    buffer[i] = 0;
  }

  Serial.println();
  Serial.println("Meter Information:");
  Serial.print("number_of_fields: "); Serial.println(energy_meter->number_of_fields);  
  Serial.print("data per meter: "); Serial.println(data_per_meter);
  Serial.print("number of meters: "); Serial.println(number_of_meters);
  Serial.print("buffer size: "); Serial.println(buffer_size);

  last_update_time = millis();
}

String modbus_result_to_string(Modbus::ResultCode result_code) {
  switch(result_code) {
    case Modbus::ResultCode::EX_SUCCESS:
      return "EX_SUCCESS";
    case Modbus::ResultCode::EX_ILLEGAL_FUNCTION:
      return "EX_ILLEGAL_FUNCTION";
    case Modbus::ResultCode::EX_ILLEGAL_ADDRESS:
      return "EX_ILLEGAL_ADDRESS";  
    case Modbus::ResultCode::EX_ILLEGAL_VALUE:
      return "EX_ILLEGAL_VALUE";  
    case Modbus::ResultCode::EX_SLAVE_FAILURE:
      return "EX_SLAVE_FAILURE";  
    case Modbus::ResultCode::EX_ACKNOWLEDGE:
      return "EX_ACKNOWLEDGE";  
    case Modbus::ResultCode::EX_SLAVE_DEVICE_BUSY:
      return "EX_SLAVE_DEVICE_BUSY";  
    case Modbus::ResultCode::EX_MEMORY_PARITY_ERROR:
      return "EX_MEMORY_PARITY_ERROR";  
    case Modbus::ResultCode::EX_PATH_UNAVAILABLE:
      return "EX_PATH_UNAVAILABLE";  
    case Modbus::ResultCode::EX_DEVICE_FAILED_TO_RESPOND:
      return "EX_DEVICE_FAILED_TO_RESPOND";  
    case Modbus::ResultCode::EX_GENERAL_FAILURE:
      return "EX_GENERAL_FAILURE";  
    case Modbus::ResultCode::EX_DATA_MISMACH:
      return "EX_DATA_MISMACH";  
    case Modbus::ResultCode::EX_UNEXPECTED_RESPONSE:
      return "EX_UNEXPECTED_RESPONSE";  
    case Modbus::ResultCode::EX_TIMEOUT:
      return "EX_TIMEOUT";  
    case Modbus::ResultCode::EX_CONNECTION_LOST:
      return "EX_CONNECTION_LOST";  
    case Modbus::ResultCode::EX_CANCEL:
      return "EX_CANCEL"; 
    default:
      return "Unknown Error";
  }
}

bool cbWrite(Modbus::ResultCode result_code, uint16_t transactionId, void* data) {
  debug_print("Request result: ");
  debug_print(modbus_result_to_string(result_code));
  debug_print(", Mem: ");
  debug_println(ESP.getFreeHeap());

  return true;
}

void waitForResult() {
    while(mb1.slave()) {
      mb1.task();
    }
}

void read_and_get(RegisterType reg_type, uint16_t meter_id, uint16_t address, uint16_t num_regs, uint16_t* data) {
  if (reg_type == RegisterType::Ireg) {
    mb1.readIreg(meter_id, address, data, num_regs, cbWrite);
    waitForResult();
  } else if (reg_type == RegisterType::Hreg) {
    mb1.readHreg(meter_id, address, data, num_regs, cbWrite);
    waitForResult();
  }
}

void reconfigure() {
  /*
  mqtt_broker = config_mqtt_broker();
  mqtt_user = config_mqtt_user();
  mqtt_password = config_mqtt_password();
  mqtt_port = config_mqtt_port();

  meter_type = config_meter_type();
  modbus_id = config_modbus_id();
  number_of_meters = config_number_of_meters();
  baudrate = config_baudrate();

  client = std::make_shared<PubSubClient>(mqtt_broker.c_str(), mqtt_port, *wifiClient);
  */

  auto restart_time = millis() + 5000;
  while (millis() < restart_time) {
    web_config_loop();
  }
  EspClass::restart();
}

void reconnect()
{
  web_config_loop();
  if (config_updated()) {
    reconfigure();
  }

  while (!client->connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client->connect(hostname.c_str(), mqtt_user.c_str(), mqtt_password.c_str(), (hostname + "/available").c_str(), 0, true, "offline"))
    {
      Serial.println("connected");
      client->publish((hostname + "/available").c_str(), "online", true);
      led_state = false;
      digitalWrite(LED_BUILTIN, led_state ? HIGH : LOW);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client->state());

      Serial.println(" try again in 5 seconds");
      auto start_time = millis();
      uint32_t blink_duration = 100;
      while (millis() - start_time < 5000) {
        if (is_in_ap_mode()) {
          blink_duration = 100;
        }
        if (is_in_network_mode()) {
          blink_duration = 50;
        }
        led_state = (millis() / blink_duration) % 2 == 0;
        digitalWrite(LED_BUILTIN, led_state ? HIGH : LOW);
        web_config_loop();
        if (config_updated()) {
          reconfigure();
        }
      }
    }
  }
}

String get_value_as_string(uint16_t* data, FieldType type, float field_factor) {
  if (type == FieldType::float32) {
    return String(get_float(data)*field_factor);
  } else if (type == FieldType::float32_reversed) {
    return String(get_float_reversed(data)*field_factor);
  } else if (type == FieldType::int16) {
    return String(get_int16_t(data)*field_factor);
  } else if (type == FieldType::int32) {
    return String(get_int32_t(data)*field_factor);
  } else if (type == FieldType::uint16) {
    return String(get_uint16_t(data)*field_factor);
  } else if (type == FieldType::uint32) {
    return String(get_uint32_t(data)*field_factor);
  } else if (type == FieldType::uint8_high_byte) {
    return String(get_uint8_t_high_byte(data)*field_factor);
  } else if (type == FieldType::uint8_low_byte) {
    return String(get_uint8_t_low_byte(data)*field_factor);
  } else {
    return "Unknown Type";
  }
}

void loop() {
  // Ensure Connection
  if (mqtt_enabled) {
    reconnect();
  }

  // Read Modbus Registers
  for(int i = 0; i < number_of_meters; i++) {
    for(const auto& run : energy_meter->run_list) {
      uint16_t buffer_address = (i*data_per_meter + run.data_map);
      uint16_t* data = buffer + buffer_address;
      read_and_get(energy_meter->register_type, i+1, run.start_address, run.number_of_words, data);
    }
  }

  for (int i = 0; i < number_of_meters; i++) {
    String energy_meter_route = topic + (i+1) + String("/");
    //debug_print("Meter Data #"); debug_println(i+1);
    for (int j = 0; j < energy_meter->number_of_fields; j++) {
      if(energy_meter->field_enabled[j]) {
        // Parse value
        uint16_t buffer_address = data_per_meter*i + energy_meter->field_data_map[j];
        //debug_print("Reading from buffer address: "); debug_println(buffer_address);
        uint16_t* data = buffer + buffer_address;
        String value = get_value_as_string(data, energy_meter->field_data_type[j], energy_meter->field_factor[j]);

        // Print debug output
        //debug_print(energy_meter->field_description[j]); debug_print(": "); debug_print(value); debug_println(energy_meter->field_unit[j]);

        // Publish value to MQTT
        if (mqtt_enabled) {
          String route = energy_meter_route + energy_meter->field_name[j];
          client->publish(route.c_str(), value.c_str(), true);
        }
      }
    }

    //debug_println("---------------");
  }

  unsigned long current_time = millis();
  unsigned long time_to_update = current_time - last_update_time;
  last_update_time = current_time;
  client->publish((topic + "time_to_update").c_str(), String(time_to_update).c_str(), true);
}