#pragma once

#include "energy_meter_config.hpp"

class Sdm72dmv1 : public EnergyMeterConfig {
public:
    Sdm72dmv1() : EnergyMeterConfig(RegisterType::Ireg) {
        max_run_length = 20;
    }

    virtual void setup_registers() override {
        add_field("total_power",            0x0034, "Total Power",            "W",   1.0f, FieldType::float32_reversed);
        add_field("total_power",            0x0034, "Total Power",            "W",   1.0f, FieldType::float32_reversed);
        add_field("import_since_reset",     0x0048, "Import Since Reset",     "kWh", 1.0f, FieldType::float32_reversed);
        add_field("export_since_reset",     0x004A, "Export Since Reset",     "kWh", 1.0f, FieldType::float32_reversed);
        add_field("total_energy",           0x0156, "Total Energy",           "kWh", 1.0f, FieldType::float32_reversed);
        add_field("settable_total_energy",  0x0180, "Settable Total Energy",  "kWh", 1.0f, FieldType::float32_reversed);
        add_field("settable_import_energy", 0x0184, "Settable Import Energy", "kWh", 1.0f, FieldType::float32_reversed);
        add_field("settable_export_energy", 0x0186, "Settable Export Energy", "kWh", 1.0f, FieldType::float32_reversed);
        add_field("import_power",           0x0500, "Import Power",           "W",   1.0f, FieldType::float32_reversed);
        add_field("export_power",           0x0502, "Export Power",           "W",   1.0f, FieldType::float32_reversed);
    }
};