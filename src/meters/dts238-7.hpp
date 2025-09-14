#pragma once

#include "energy_meter_config.hpp"
#include "energy_meter_type.hpp"

class Dts238_7 : public EnergyMeterConfig {
public:
    Dts238_7() : EnergyMeterConfig(RegisterType::Hreg) {
        max_run_length = 25;
        auto_config = false;
    }

    virtual void setup_registers() override {
        add_field("total_energy_count",        0x00, "Total Energy Count",        "kWh", 0.01f,  FieldType::uint32);
        add_field("total_export_energy_count", 0x08, "Total Export Energy Count", "kWh", 0.01f,  FieldType::uint32);
        add_field("total_import_energy_count", 0x0A, "Total Import Energy Count", "kWh", 0.01f,  FieldType::uint32);
        add_field("frequency",                 0x11, "Frequency",                 "Hz",  0.01f,  FieldType::uint16);
        add_field("communication_address",     0x15, "Communication address",     "",    1.0f,   FieldType::uint8_high_byte);
        add_field("baudrate",                  0x15, "Baudrate",                  "",    1.0f,   FieldType::uint8_low_byte);
        add_field("voltage_l1",                0x80, "Voltage L1",                "V",   0.1f,   FieldType::uint16);
        add_field("voltage_l2",                0x81, "Voltage L2",                "V",   0.1f,   FieldType::uint16);
        add_field("voltage_l3",                0x82, "Voltage L3",                "V",   0.1f,   FieldType::uint16);
        add_field("current_l1",                0x83, "Current L1",                "A",   0.01f,  FieldType::uint16);
        add_field("current_l2",                0x84, "Current L2",                "A",   0.01f,  FieldType::uint16);
        add_field("current_l3",                0x85, "Current L3",                "A",   0.01f,  FieldType::uint16);
        add_field("total_active_power",        0x86, "Total Active Power",        "W",   1.0f,   FieldType::int32);
        add_field("active_power_l1",           0x88, "Active Power L1",           "W",   1.0f,   FieldType::int16);
        add_field("active_power_l2",           0x89, "Active Power L2",           "W",   1.0f,   FieldType::int16);
        add_field("active_power_l3",           0x8A, "Active Power L3",           "W",   1.0f,   FieldType::int16);
        add_field("total_reactive_power",      0x8B, "Total Reactive Power",      "W",   1.0f,   FieldType::int32);
        add_field("reactive_power_l1",         0x8D, "Reactive Power L1",         "W",   1.0f,   FieldType::int16);
        add_field("reactive_power_l2",         0x8E, "Reactive Power L2",         "W",   1.0f,   FieldType::int16);
        add_field("reactive_power_l3",         0x8F, "Reactive Power L3",         "W",   1.0f,   FieldType::int16);
        add_field("total_apparent_power",      0x90, "Total Apparent Power",      "W",   1.0f,   FieldType::uint32);
        add_field("apparent_power_l1",         0x92, "Apparent Power L1",         "W",   1.0f,   FieldType::uint16);
        add_field("apparent_power_l2",         0x93, "Apparent Power L2",         "W",   1.0f,   FieldType::uint16);
        add_field("apparent_power_l3",         0x94, "Apparent Power L3",         "W",   1.0f,   FieldType::uint16);
        add_field("total_power_factor",        0x95, "Total Power Factor",        "",    0.001f, FieldType::uint16);
        add_field("power_factor_l1",           0x96, "Power Factor L1",           "",    0.001f, FieldType::uint16);
        add_field("power_factor_l2",           0x97, "Power Factor L2",           "",    0.001f, FieldType::uint16);
        add_field("power_factor_l3",           0x98, "Power Factor L3",           "",    0.001f, FieldType::uint16);

        run_list.push_back(FieldsRun(0x00, 22, 0));
        run_list.push_back(FieldsRun(0x80, 25, 22));
        buffer_size = 22 + 25;

        for (int i = 0; i <= 5; i++) {
            fields[i].buffer_position = fields[i].address;
        }

        for (int i = 6; i <= 27; i++) {
            fields[i].buffer_position = fields[i].address - 0x80 + 22;
        }
    }
};