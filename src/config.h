#pragma once

#include <Arduino.h>
#include "devices.hpp"

const char* root_topic = "modbus-to-mqtt";
const char* wifi_ssid = "ArneTheaHome3";
const char* wifi_pw = "dk290dKe-.-";
const char* mqtt_broker = "octoprint.fritz.box";
const char* mqtt_user = "";
const char* mqtt_password = "";
const uint16_t mqtt_port = 1883;
const bool mqtt_enabled = true;
const uint8_t modbus_id = 1;
const DeviceType device_type = DeviceType::Growatt_MIC;