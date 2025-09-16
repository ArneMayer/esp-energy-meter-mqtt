#pragma once

#include <Arduino.h>
#include "devices.hpp"

String root_topic = "energy-meter-rs485";
String wifi_ssid = "";
String wifi_pw = "";
String mqtt_broker = "octopi2.fritz.box";
String mqtt_user = "";
String mqtt_password = "";
const uint16_t mqtt_port = 1883;
const uint8_t number_of_devices = 1;
const DeviceType device_type = DeviceType::SDM72D_M_V2;