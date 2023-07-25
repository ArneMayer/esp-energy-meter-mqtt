#pragma once

#include <Arduino.h>
#include "energy_meter_type.hpp"

const char* wifi_ssid = "";
const char* wifi_pw = "";
const char* mqtt_broker = "octopi2.fritz.box";
const char* mqtt_user = "";
const char* mqtt_password = "";
const uint16_t mqtt_port = 1883;
const uint8_t number_of_meters = 1;
const EnergyMeterType meter_type = EnergyMeterType::SDM72D_M_V2;
const uint32_t baudrate = 9600;
const uint8_t modbus_id = 1;