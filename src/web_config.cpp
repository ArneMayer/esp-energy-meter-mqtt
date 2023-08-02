#include "web_config.hpp"

#include <IotWebConf.h>
#include <memory>
#include "util.hpp"

using iotwebconf::CheckboxParameter;
using iotwebconf::NumberParameter;
using iotwebconf::ParameterGroup;
using iotwebconf::SelectParameter;
using iotwebconf::TextParameter;

String thingName = String("energy-" + mac_string());
String wifiInitialApPassword = "";

#define STRING_LEN 128
#define NUMBER_LEN 32

// -- Configuration specific key. The value should be modified if config structure was changed.
#define CONFIG_VERSION "v5"

// -- When CONFIG_PIN is pulled to ground on startup, the Thing will use the initial
//      password to buld an AP. (E.g. in case of lost password)
#define CONFIG_PIN D2

// -- Status indicator pin.
//      First it will light up (kept LOW), on Wifi connection it will blink,
//      when connected to the Wifi it will turn off (kept HIGH).
#define STATUS_PIN LED_BUILTIN

void handleRoot();
void configSaved();
bool formValidator(iotwebconf::WebRequestWrapper* webRequestWrapper);
bool _new_config = false;

DNSServer dnsServer;
WebServer server(80);

char id_config_value[NUMBER_LEN];
char number_of_meters_config_value[NUMBER_LEN];
char baudrate_config_value[STRING_LEN];
char meter_type_config_value[STRING_LEN];

char mqtt_broker_config_value[STRING_LEN];
char mqtt_user_config_value[STRING_LEN];
char mqtt_pw_config_value[STRING_LEN];
char mqtt_port_config_value[NUMBER_LEN];

static char baudrate_values[][STRING_LEN] = {"2400", "4800", "9600", "19200", "38400"};
const auto num_baudrate_values = sizeof(baudrate_values) / STRING_LEN;

static char meter_type_values[][STRING_LEN] = {"SDM72D_M_V2", "SDM72D_M_V1", "DTS238_7", "SDM630_V2"/*, "DTSU666"*/};
const auto num_meter_type_values = sizeof(meter_type_values) / STRING_LEN;

std::shared_ptr<IotWebConf> iotWebConf;

auto group_modbus = ParameterGroup("group_modbus", "Modbus");
auto group_mqtt = ParameterGroup("group_mqtt", "MQTT");

auto id_config = NumberParameter("Modbus ID", "id_config", id_config_value, NUMBER_LEN, "1", "1..255", "min='1' max='255' step='1'");
auto number_of_meters_config = NumberParameter("Number of Meters", "number_of_meters_config", number_of_meters_config_value, NUMBER_LEN, "1", "1..255", "min='1' max='255' step='1'");
auto baudrate_config = SelectParameter("Baudrate", "baudrate_config", baudrate_config_value, STRING_LEN, (char*)baudrate_values, (char*)baudrate_values, num_baudrate_values, STRING_LEN, baudrate_values[2]);
auto meter_type_config = SelectParameter("Energy Meter Type", "meter_type_config", meter_type_config_value, STRING_LEN, (char*)meter_type_values, (char*)meter_type_values, num_meter_type_values, STRING_LEN, meter_type_values[0]);

auto mqtt_broker_config = TextParameter("MQTT Broker", "mqtt_broker_config", mqtt_broker_config_value, STRING_LEN);
auto mqtt_user_config = TextParameter("MQTT User", "mqtt_user_config", mqtt_user_config_value, STRING_LEN);
auto mqtt_pw_config = TextParameter("MQTT Password", "mqtt_pw_config", mqtt_pw_config_value, STRING_LEN);
auto mqtt_port_config = NumberParameter("MQTT Port", "mqtt_port_config", mqtt_port_config_value, NUMBER_LEN, "1883", "0..65535", "min='0' max='65535' step='1'");

void web_config_setup() {
    iotWebConf = std::make_shared<IotWebConf>(thingName.c_str(), &dnsServer, &server, wifiInitialApPassword.c_str(), CONFIG_VERSION);

    group_modbus.addItem(&id_config);
    group_modbus.addItem(&number_of_meters_config);
    group_modbus.addItem(&baudrate_config);
    group_modbus.addItem(&meter_type_config);

    group_mqtt.addItem(&mqtt_broker_config);
    group_mqtt.addItem(&mqtt_user_config);
    group_mqtt.addItem(&mqtt_pw_config);
    group_mqtt.addItem(&mqtt_port_config);

    iotWebConf->setStatusPin(-1);
    iotWebConf->setConfigPin(-1);
    iotWebConf->addParameterGroup(&group_mqtt);
    iotWebConf->addParameterGroup(&group_modbus);
    iotWebConf->setConfigSavedCallback(&configSaved);
    iotWebConf->setFormValidator(&formValidator);
    iotWebConf->getApTimeoutParameter()->visible = false;
    iotWebConf->getThingNameParameter()->label = "Device Name";

    iotWebConf->init();

    server.on("/", [] { iotWebConf->handleConfig(); });
    server.onNotFound([]() { iotWebConf->handleConfig(); });
}

void configSaved() {
    Serial.println("Configuration was updated.");
    _new_config = true;
}

bool formValidator(iotwebconf::WebRequestWrapper* webRequestWrapper) {
    Serial.println("Validating form.");
    bool valid = true;

    return valid;
}

void web_config_loop() {
    iotWebConf->doLoop();
}

bool config_updated() {
    return _new_config;
}

/*
String config_wifi_ssid() {
}

String config_wifi_pw() {
}
*/

String config_mqtt_broker() {
    return String(mqtt_broker_config_value);
}

String config_mqtt_user() {
    return String(mqtt_user_config_value);
}

String config_mqtt_password() {
    return String(mqtt_pw_config_value);
}

uint16_t config_mqtt_port() {
    return String(mqtt_port_config_value).toInt();
}

uint8_t config_modbus_id() {
    return String(id_config_value).toInt();
}

uint8_t config_number_of_meters() {
    return String(number_of_meters_config_value).toInt();
}

uint32_t config_baudrate() {
    return String(baudrate_config_value).toInt();
}

EnergyMeterType config_meter_type() {
    return string_to_meter_type(meter_type_config_value);
}

