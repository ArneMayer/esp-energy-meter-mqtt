#pragma once

#include <Arduino.h>
#include "devices.hpp"
#include "modbus/modbus_connection.hpp"

const char* root_topic = "modbus-to-mqtt";
const char* wifi_ssid = "";
const char* wifi_pw = "";
const char* mqtt_broker = "";
const char* mqtt_user = "";
const char* mqtt_password = "";
const uint16_t mqtt_port = 1883;
const bool mqtt_enabled = true;

const std::vector<std::pair<ModbusId, DeviceType>> device_configs = {
    {1, DeviceType::DTS238_7},
    {2, DeviceType::Growatt_MIC},
    {3, DeviceType::SDM630_V2},
    {4, DeviceType::SDM72D_M_V1},
    {5, DeviceType::SDM72D_M_V2}
};