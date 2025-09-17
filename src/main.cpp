#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <PubSubClient.h>

#include "debug.hpp"
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

unsigned long last_update_time = 0;

char mac[14] = {0};
char hostname[40] = {0};
char available_topic[50] = {0};
char ttu_topic[60] = {0};

void setup_wifi() {
  uint8_t mac_data[6];
  WiFi.macAddress(mac_data);
  snprintf(mac, sizeof(mac), "%02x%02x%02x%02x%02x%02x", mac_data[0], mac_data[1], mac_data[2], mac_data[3], mac_data[4], mac_data[5]);
  snprintf(hostname, sizeof(hostname), "modbus-interface-%s", mac);

  WiFi.mode(WIFI_STA);
  WiFi.hostname(hostname);
  WiFi.begin(wifi_ssid, wifi_pw);
  
  Serial.print("Connecting to WiFi.");  

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    yield();
  }

  Serial.println(" Ok!");
 
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(74880);
  Serial.println("");
  Serial.println("-----------------------------");

  auto connection = std::make_shared<ModbusConnection>(D5, D6, D4, 9600);
  setup_wifi();

  client.setServer(mqtt_broker, mqtt_port);
  client.setClient(wifiClient);

  snprintf(available_topic, sizeof(available_topic), "%s/available", root_topic);
  snprintf(ttu_topic, sizeof(ttu_topic), "%s/time_to_update", root_topic);

  Serial.print("Root Topic: ");
  Serial.println(root_topic);

  // Setup Configuration
  Serial.print("Configured Modbus Device: ");
  Serial.println(to_string(device_type));

  if(device_type == DeviceType::SDM72D_M_V2) {
    devices.push_back(Sdm72dmv2(connection, modbus_id));
  } 
  else if (device_type == DeviceType::SDM72D_M_V1) {
    devices.push_back(Sdm72dmv1(connection, modbus_id));
  } 
  else if (device_type == DeviceType::DTS238_7) {
    devices.push_back(Dts238_7(connection, modbus_id));
  }
  else if (device_type == DeviceType::SDM630_V2) {
    devices.push_back(Sdm630v2(connection, modbus_id));
  }
  else if (device_type == DeviceType::Growatt_MIC) {
    devices.push_back(GrowattMic(connection, modbus_id));
  }
  else {
    halt();
  }

  last_update_time = millis();
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(hostname, mqtt_user, mqtt_password, available_topic, 0, true, "offline"))
    {
      Serial.println(" Ok!");
      client.publish(available_topic, "online", true);
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

  char value_str[32] = {0};
  char field_topic[128] = {0};
  char device_topic[64] = {0};
  char ttu_str[16] = {0};

  for (ModbusDevice& device : devices) {
    device.update_all();

    snprintf(device_topic, sizeof(device_topic), "%s/%u/", root_topic, device.modbus_id);
    for (const auto& field_value : device.values()) {
      const auto& field = field_value.first;
      float value = field_value.second;

      if(field.enabled) {
        // Print debug output
        //debug_print(field.description); debug_print(": "); debug_print(value); debug_println(field.unit);

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
  client.publish(ttu_topic, ttu_str, true);
}