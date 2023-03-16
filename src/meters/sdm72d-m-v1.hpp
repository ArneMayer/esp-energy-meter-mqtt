#pragma once

#include "energy_meter_config.hpp"

#define NUM_FIELDS_V1 9

class Sdm72dmv1 : public EnergyMeterConfig {
public:
    Sdm72dmv1() : EnergyMeterConfig(NUM_FIELDS_V1, RegisterType::Ireg) {
        max_run_length = 20;
    }

    virtual void setup_registers() override {
        for(int i = 0; i < number_of_fields; i++) {
            field_data_type[i] = FieldType::float32_reversed;
            field_factor[i] = 1.0f;
            field_enabled[i] = true;
        }

        field_name[0] = "total_power";            field_address[0] = 0x0034; field_description[0] = "Total Power";            field_unit[0] = "W";
        field_name[1] = "import_since_reset";     field_address[1] = 0x0048; field_description[1] = "Import Since Reset";     field_unit[1] = "kWh";
        field_name[2] = "export_since_reset";     field_address[2] = 0x004A; field_description[2] = "Export Since Reset";     field_unit[2] = "kWh";
        field_name[3] = "total_energy";           field_address[3] = 0x0156; field_description[3] = "Total Energy";           field_unit[3] = "kWh";
        field_name[4] = "settable_total_energy";  field_address[4] = 0x0180; field_description[4] = "Settable Total Energy";  field_unit[4] = "kWh";
        field_name[5] = "settable_import_energy"; field_address[5] = 0x0184; field_description[5] = "Settable Import Energy"; field_unit[5] = "kWh";
        field_name[6] = "settable_export_energy"; field_address[6] = 0x0186; field_description[6] = "Settable Export Energy"; field_unit[6] = "kWh";
        field_name[7] = "import_power";           field_address[7] = 0x0500; field_description[7] = "Import Power";           field_unit[7] = "W";
        field_name[8] = "export_power";           field_address[8] = 0x0502; field_description[8] = "Export Power";           field_unit[8] = "W";
    }
};

/*
constexpr uint8_t V1_TOTAL_POWER = 0;
constexpr uint8_t V1_IMPORT_SINCE_RESET = 1;
constexpr uint8_t V1_EXPORT_SINCE_RESET = 2;
constexpr uint8_t V1_TOTAL_ENERGY = 3;
constexpr uint8_t V1_SETTABLE_TOTAL_ENERGY = 4;
constexpr uint8_t V1_SETTABLE_IMPORT_ENERGY = 5;
constexpr uint8_t V1_SETTABLE_EXPORT_ENERGY = 6;
constexpr uint8_t V1_IMPORT_POWER = 7;
constexpr uint8_t V1_EXPORT_POWER = 8;
*/