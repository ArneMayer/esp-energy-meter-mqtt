#pragma once

#include "energy_meter_config.hpp"
#include "energy_meter_type.hpp"

#define NUM_FIELDS_DTS238_7 28

class Dts238_7 : public EnergyMeterConfig {
public:
    Dts238_7() : EnergyMeterConfig(NUM_FIELDS_DTS238_7, RegisterType::Hreg) {
        max_run_length = 25;
        auto_config = false;
    }

    virtual void setup_registers() override {
        for(int i = 0; i < number_of_fields; i++) {
            field_factor[i] = 1.0f;
            field_enabled[i] = true;
        }

        field_name[0] = "total_energy_count";        field_address[0] = 0x00;  field_description[0] = "Total Energy Count";        field_unit[0] = "kWh"; field_data_type[0] = FieldType::uint32; field_factor[0] = 0.01f;
        field_name[1] = "total_export_energy_count"; field_address[1] = 0x08;  field_description[1] = "Total Export Energy Count"; field_unit[1] = "kWh"; field_data_type[1] = FieldType::uint32; field_factor[1] = 0.01f;
        field_name[2] = "total_import_energy_count"; field_address[2] = 0x0A;  field_description[2] = "Total Import Energy Count"; field_unit[2] = "kWh"; field_data_type[2] = FieldType::uint32; field_factor[2] = 0.01f;
        field_name[3] = "frequency";                 field_address[3] = 0x11;  field_description[3] = "Frequency";                 field_unit[3] = "Hz";  field_data_type[3] = FieldType::uint16; field_factor[3] = 0.01f;
        field_name[4] = "communication_address";     field_address[4] = 0x15;  field_description[4] = "Communication address";     field_unit[4] = "";    field_data_type[4] = FieldType::uint8_high_byte;
        field_name[5] = "baudrate";                  field_address[5] = 0x15;  field_description[5] = "Baudrate";                  field_unit[5] = "";    field_data_type[5] = FieldType::uint8_low_byte;
        field_name[6] = "voltage_l1";                field_address[6] = 0x80;  field_description[6] = "Voltage L1";                field_unit[6] = "V";   field_data_type[6] = FieldType::uint16; field_factor[6] = 0.1f;
        field_name[7] = "voltage_l2";                field_address[7] = 0x81;  field_description[7] = "Voltage L2";                field_unit[7] = "V";   field_data_type[7] = FieldType::uint16; field_factor[7] = 0.1f;
        field_name[8] = "voltage_l3";                field_address[8] = 0x82;  field_description[8] = "Voltage L3";                field_unit[8] = "V";   field_data_type[8] = FieldType::uint16; field_factor[8] = 0.1f;
        field_name[9] = "current_l1";                field_address[9] = 0x83;  field_description[9] = "Current L1";                field_unit[9] = "A";   field_data_type[9] = FieldType::uint16; field_factor[9] = 0.01f;
        field_name[10] = "current_l2";               field_address[10] = 0x84; field_description[10] = "Current L3";               field_unit[10] = "A";  field_data_type[10] = FieldType::uint16; field_factor[10] = 0.01f;
        field_name[11] = "current_l3";               field_address[11] = 0x85; field_description[11] = "Current L3";               field_unit[11] = "A";  field_data_type[11] = FieldType::uint16; field_factor[11] = 0.01f;
        field_name[12] = "total_active_power";       field_address[12] = 0x86; field_description[12] = "Total Active Power";       field_unit[12] = "W";  field_data_type[12] = FieldType::int32;
        field_name[13] = "active_power_l1";          field_address[13] = 0x88; field_description[13] = "Active Power L1";          field_unit[13] = "W";  field_data_type[13] = FieldType::int16;
        field_name[14] = "active_power_l2";          field_address[14] = 0x89; field_description[14] = "Active Power L2";          field_unit[14] = "W";  field_data_type[14] = FieldType::int16;
        field_name[15] = "active_power_l3";          field_address[15] = 0x8A; field_description[15] = "Active Power L3";          field_unit[15] = "W";  field_data_type[15] = FieldType::int16;
        field_name[16] = "total_reactive_power";     field_address[16] = 0x8B; field_description[16] = "Total Reactive Power";     field_unit[16] = "W";  field_data_type[16] = FieldType::int32;
        field_name[17] = "reactive_power_l1";        field_address[17] = 0x8D; field_description[17] = "Reactive Power L1";        field_unit[17] = "W";  field_data_type[17] = FieldType::int16;
        field_name[18] = "reactive_power_l2";        field_address[18] = 0x8E; field_description[18] = "Reactive Power L2";        field_unit[18] = "W";  field_data_type[18] = FieldType::int16;
        field_name[19] = "reactive_power_l3";        field_address[19] = 0x8F; field_description[19] = "Reactive Power L3";        field_unit[19] = "W";  field_data_type[19] = FieldType::int16;
        field_name[20] = "total_apparent_power";     field_address[20] = 0x90; field_description[20] = "Total Apparent Power";     field_unit[20] = "W";  field_data_type[20] = FieldType::uint32;
        field_name[21] = "apparent_power_l1";        field_address[21] = 0x92; field_description[21] = "Apparent Power L1";        field_unit[21] = "W";  field_data_type[21] = FieldType::uint16;
        field_name[22] = "apparent_power_l2";        field_address[22] = 0x93; field_description[22] = "Apparent Power L2";        field_unit[22] = "W";  field_data_type[22] = FieldType::uint16;
        field_name[23] = "apparent_power_l3";        field_address[23] = 0x94; field_description[23] = "Apparent Power L3";        field_unit[23] = "W";  field_data_type[23] = FieldType::uint16;
        field_name[24] = "total_power_factor";       field_address[24] = 0x95; field_description[24] = "Total Power Factor";       field_unit[24] = "";   field_data_type[24] = FieldType::uint16; field_factor[24] = 0.001f;
        field_name[25] = "power_factor_l1";          field_address[25] = 0x96; field_description[25] = "Power Factor L1";          field_unit[25] = "";   field_data_type[25] = FieldType::uint16; field_factor[25] = 0.001f;
        field_name[26] = "power_factor_l2";          field_address[26] = 0x97; field_description[26] = "Power Factor L2";          field_unit[26] = "";   field_data_type[26] = FieldType::uint16; field_factor[26] = 0.001f;
        field_name[27] = "power_factor_l3";          field_address[27] = 0x98; field_description[27] = "Power Factor L3";          field_unit[27] = "";   field_data_type[27] = FieldType::uint16; field_factor[27] = 0.001f;

        run_list.push_back(FieldsRun(0x00, 22, 0));
        run_list.push_back(FieldsRun(0x80, 25, 22));
        data_per_meter = 22 + 25;

        for (int i = 0; i <= 5; i++) {
            field_data_map[i] = field_address[i];
        }

        for (int i = 6; i <= 27; i++) {
            field_data_map[i] = field_address[i] - 0x80 + 22;
        }
    }
};