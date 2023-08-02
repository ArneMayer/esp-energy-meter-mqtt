#pragma once

#include <Arduino.h>
#include "energy_meter_type.hpp"

// String config_wifi_ssid();
// String config_wifi_pw();

String config_mqtt_broker();
String config_mqtt_user();
String config_mqtt_password();
uint16_t config_mqtt_port();

uint8_t config_modbus_id();
uint8_t config_number_of_meters();
uint32_t config_baudrate();
EnergyMeterType config_meter_type();

void web_config_setup();
void web_config_loop();
bool config_updated();