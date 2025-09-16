#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <PubSubClient.h>

#include "logging.hpp"
#include "config.h"
#include "modbus_device.hpp"
#include "devices/sdm72d_m_v1.hpp"
#include "devices/sdm72d_m_v2.hpp"
#include "devices/dts238_7.hpp"
#include "devices/sdm630_v2.hpp"
#include "devices/growatt_mic.hpp"
#include "modbus_connection.hpp"

WiFiClient wifiClient;
PubSubClient client(mqtt_broker.c_str(), mqtt_port, wifiClient);

std::vector<ModbusDevice> devices;

bool mqtt_enabled = true;

unsigned long last_update_time = 0;
String hostname;

String mac_string() {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char mac_string[6 * 2 + 1] = {0};
    snprintf(mac_string, 6 * 2 + 1, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(mac_string);
}

void setup_wifi() {
  String deviceId = mac_string();
  String hostname = String("modbus-interface-" + deviceId);
  Serial.print("Root Topic: ");
  Serial.println(root_topic);

  WiFi.mode(WIFI_STA);
  WiFi.hostname(hostname);
  WiFi.begin(wifi_ssid, wifi_pw);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

 

void setup() {
  Serial.begin(74880);
  auto connection = std::make_shared<ModbusConnection>(D5, D6, D4, 9600);
  setup_wifi();

  // Setup Configuration
  if(device_type == DeviceType::SDM72D_M_V2) {
    Serial.println("Configured Energy Meter Type: SDM72D_M_V2");
    devices.push_back(Sdm72dmv2(connection, 1));
  } 
  else if (device_type == DeviceType::SDM72D_M_V1) {
    Serial.println("Configured Energy Meter Type: SDM72D_M_V1");
    devices.push_back(Sdm72dmv1(connection, 1));
  } 
  else if (device_type == DeviceType::DTS238_7) {
    Serial.println("Configured Energy Meter Type: DTS238_7");
    devices.push_back(Dts238_7(connection, 1));
  }
  else if (device_type == DeviceType::SDM630_V2) {
    Serial.println("Configured Energy Meter Type: SDM630_V2");
    devices.push_back(Sdm630v2(connection, 1));
  }
  else if (device_type == DeviceType::Growatt_MIC) {
    Serial.println("Configured Energy Meter Type: Growatt_MIC");
    devices.push_back(GrowattMic(connection, 1));
  }
  else {
    Serial.println("Unknown Configuration, defaulting to SDM72D_M_V1");
    devices.push_back(Sdm72dmv1(connection, 1));
  }

  

  //Serial.print("number_of_fields: "); Serial.println(modbus_device->fields.size());  
  //Serial.print("data per device: "); Serial.println(data_per_meter);
  Serial.print("number of devices: "); Serial.println(number_of_devices);
  //Serial.print("buffer size: "); Serial.println(buffer_size);

  last_update_time = millis();
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(hostname.c_str(), mqtt_user.c_str(), mqtt_password.c_str(), (root_topic + "/available").c_str(), 0, true, "offline"))
    {
      Serial.println("connected");
      client.publish((root_topic + "/available").c_str(), "online", true);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());

      Serial.println(" try again in 3 seconds");
      delay(3000);
      return;
    }
  }
}

void loop() {
  // Ensure Connection
  if (mqtt_enabled) {
    reconnect();
  }

  for (ModbusDevice& device : devices) {
    device.update_all();

    String device_topic = root_topic + device.modbus_id + String("/");
    for (const auto& field_value : device.values()) {
      const auto& field = field_value.first;
      const auto& value = field_value.second;

      if(field.enabled) {
        // Print debug output
        debug_print(field.description); debug_print(": "); debug_print(value); debug_println(field.unit);

        // Publish value to MQTT
        if (mqtt_enabled) {
          String field_topic = device_topic + field.name;
          client.publish(field_topic.c_str(), value.c_str(), true);
        }
      }
    }

    //debug_println("---------------");
  }

  unsigned long current_time = millis();
  unsigned long time_to_update = current_time - last_update_time;
  last_update_time = current_time;
  client.publish((root_topic + "time_to_update").c_str(), String(time_to_update).c_str(), true);
}