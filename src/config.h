#pragma once

#include <Arduino.h>
#include "devices.hpp"

const char* wifi_ssid = "";
const char* wifi_pw = "";
const char* mqtt_broker = "octopi2.fritz.box";
const char* mqtt_user = "";
const char* mqtt_password = "";
const uint16_t mqtt_port = 1883;
const uint8_t number_of_devices = 1;
const DeviceType device_type = DeviceType::SDM72D_M_V2;