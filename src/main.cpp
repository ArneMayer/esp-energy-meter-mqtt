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
PubSubClient client;

std::vector<ModbusDevice> devices;

bool mqtt_enabled = true;

unsigned long last_update_time = 0;
String hostname;
String available_topic = String(root_topic) + "/available";
String ttu_topic = String(root_topic) + "time_to_update";

String mac_string() {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char mac_string[6 * 2 + 1] = {0};
    snprintf(mac_string, 6 * 2 + 1, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(mac_string);
}

void setup_wifi() {
  String deviceId = mac_string();
  hostname = String("modbus-interface-" + deviceId);
  Serial.print("Root Topic: ");
  Serial.println(root_topic);

  WiFi.mode(WIFI_STA);
  WiFi.hostname(hostname);
  WiFi.begin(wifi_ssid, wifi_pw);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    yield();
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

  client.setServer(mqtt_broker, mqtt_port);
  client.setClient(wifiClient);

  // Setup Configuration
  if(device_type == DeviceType::SDM72D_M_V2) {
    Serial.println("Configured Energy Meter Type: SDM72D_M_V2");
    devices.push_back(Sdm72dmv2(connection, modbus_id));
  } 
  else if (device_type == DeviceType::SDM72D_M_V1) {
    Serial.println("Configured Energy Meter Type: SDM72D_M_V1");
    devices.push_back(Sdm72dmv1(connection, modbus_id));
  } 
  else if (device_type == DeviceType::DTS238_7) {
    Serial.println("Configured Energy Meter Type: DTS238_7");
    devices.push_back(Dts238_7(connection, modbus_id));
  }
  else if (device_type == DeviceType::SDM630_V2) {
    Serial.println("Configured Energy Meter Type: SDM630_V2");
    devices.push_back(Sdm630v2(connection, modbus_id));
  }
  else if (device_type == DeviceType::Growatt_MIC) {
    Serial.println("Configured Energy Meter Type: Growatt_MIC");
    devices.push_back(GrowattMic(connection, modbus_id));
  }
  else {
    Serial.println("Unknown Configuration, defaulting to SDM72D_M_V1");
    devices.push_back(Sdm72dmv1(connection, modbus_id));
  }

  

  //Serial.print("number_of_fields: "); Serial.println(modbus_device->fields.size());  
  //Serial.print("data per device: "); Serial.println(data_per_meter);
  //Serial.print("buffer size: "); Serial.println(buffer_size);

  last_update_time = millis();
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(hostname.c_str(), mqtt_user, mqtt_password, available_topic.c_str(), 0, true, "offline"))
    {
      Serial.println("connected");
      client.publish(available_topic.c_str(), "online", true);
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
  if (mqtt_enabled) {
    reconnect();
    client.loop();
  }

  char value_str[32];
  char field_topic[128];
  char device_topic[64];
  char ttu_str[16];

  for (ModbusDevice& device : devices) {
    device.update_all();

    snprintf(device_topic, sizeof(device_topic), "%s/%u/", root_topic, device.modbus_id);
    for (const auto& field_value : device.values()) {
      const auto& field = field_value.first;
      float value = field_value.second;

      if(field.enabled) {
        // Print debug output
        debug_print(field.description); debug_print(": "); debug_print(value); debug_println(field.unit);

        // Publish value to MQTT
        if (mqtt_enabled) {
          snprintf(field_topic, sizeof(field_topic), "%s%s", device_topic, field.name);
          snprintf(value_str, sizeof(value_str), "%g", value);
          client.publish(field_topic, value_str, false);
        }
      }
    }

    //debug_println("---------------");
  }

  unsigned long current_time = millis();
  unsigned long time_to_update = current_time - last_update_time;
  last_update_time = current_time;
  snprintf(ttu_str, sizeof(ttu_str), "%lu", time_to_update);
  client.publish(ttu_topic.c_str(), ttu_str, true);
}