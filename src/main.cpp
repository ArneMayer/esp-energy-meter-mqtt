#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <PubSubClient.h>

#include "debug.hpp"
#include "config.h"
#include "modbus/modbus_device.hpp"
#include "devices/sdm72d_m_v1.hpp"
#include "devices/sdm72d_m_v2.hpp"
#include "devices/dts238_7.hpp"
#include "devices/sdm630_v2.hpp"
#include "devices/growatt_mic.hpp"
#include "modbus/modbus_connection.hpp"
#include "value_tracker.hpp"

WiFiClient wifiClient;
PubSubClient client;

std::vector<std::pair<ModbusDevice,DeviceType>> devices;

unsigned long last_update_time = 0;

char mac[14] = {0};
char hostname[48] = {0};
char available_topic[48] = {0};
char ttu_topic[64] = {0};
char free_heap_topic[64] = {0};
char max_free_block_topic[64] = {0};

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
  snprintf(free_heap_topic, sizeof(free_heap_topic), "%s/free_heap", root_topic);
  snprintf(max_free_block_topic, sizeof(max_free_block_topic), "%s/max_free_block", root_topic);
  snprintf(ttu_topic, sizeof(ttu_topic), "%s/time_to_update", root_topic);

  Serial.print("Root Topic: ");
  Serial.println(root_topic);

  // Setup Configuration
  for (const auto&[modbus_id, device_type] : device_configs) {
    Serial.print("Configured Modbus Device: ");
    Serial.print(to_string(device_type));
    Serial.print(", Modbus ID: ");
    Serial.println(modbus_id);

    if(device_type == DeviceType::SDM72D_M_V2) {
      devices.emplace_back(Sdm72dmv2(connection, modbus_id), device_type);
    } 
    else if (device_type == DeviceType::SDM72D_M_V1) {
      devices.emplace_back(Sdm72dmv1(connection, modbus_id), device_type);
    } 
    else if (device_type == DeviceType::DTS238_7) {
      devices.emplace_back(Dts238_7(connection, modbus_id), device_type);
    }
    else if (device_type == DeviceType::SDM630_V2) {
      devices.emplace_back(Sdm630v2(connection, modbus_id), device_type);
    }
    else if (device_type == DeviceType::Growatt_MIC) {
      devices.emplace_back(GrowattMic(connection, modbus_id), device_type);
    }
    else {
      halt();
    }
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

  char payload_str[48] = {0};
  char topic_str[128] = {0};

  for (auto&[device, device_type] : devices) {
    device.update_all();

    const auto&[fields, values] = device.values();

    for (size_t i = 0; i < fields.size(); i++) {
      const auto& field = fields[i];
      float value = values[i];

      // Publish value to MQTT
      if (mqtt_enabled) {
        snprintf(topic_str, sizeof(topic_str), "%s/%s/%s/value", root_topic, to_string(device_type), field.name);
        snprintf(payload_str, sizeof(payload_str), "%g", value);
        client.publish(topic_str, payload_str, false);

        snprintf(topic_str, sizeof(topic_str), "%s/%s/%s/description", root_topic, to_string(device_type), field.name);
        client.publish(topic_str, field.description, false);

        snprintf(topic_str, sizeof(topic_str), "%s/%s/%s/unit", root_topic, to_string(device_type), field.name);
        client.publish(topic_str, field.unit, false);
      }
    }
  }

  unsigned long current_time = millis();
  unsigned long time_to_update = current_time - last_update_time;
  last_update_time = current_time;
  snprintf(payload_str, sizeof(payload_str), "%lu", time_to_update);
  client.publish(ttu_topic, payload_str, true);

  snprintf(payload_str, sizeof(payload_str), "%u", ESP.getFreeHeap());
  client.publish(free_heap_topic, payload_str, true);

  snprintf(payload_str, sizeof(payload_str), "%u", ESP.getMaxFreeBlockSize());
  client.publish(max_free_block_topic, payload_str, true);
}