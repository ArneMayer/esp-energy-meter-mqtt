#pragma once

#include <Arduino.h>
#include "energy_meter_type.hpp"

const char* wifi_ssid = "Vodafone-A054";
const char* wifi_pw = "ZfPsTdk74daGbGFp";
const char* mqtt_broker = "octopi2.fritz.box";
const char* mqtt_user = "";
const char* mqtt_password = "";
const uint16_t mqtt_port = 1883;
const uint8_t number_of_meters = 1;
const EnergyMeterType meter_type = EnergyMeterType::SDM630_V2;