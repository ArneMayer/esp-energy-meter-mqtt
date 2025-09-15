#pragma once

#include "energy_meter_config.hpp"

class Sdm72dmv2 : public EnergyMeterConfig {
public:
    Sdm72dmv2() : EnergyMeterConfig(RegisterType::Ireg) {
        max_chunk_size = 20;
    }

    void add_field(String name, uint16_t address, String description, String unit) {
        EnergyMeterConfig::add_field(name, address, description, unit, 1.0f, FieldType::float32_reversed, true);
    }

    virtual void setup_registers() override {
        add_field("phase1_voltage", 0x0000, "Phase 1 Voltage", "V");
        add_field("phase2_voltage", 0x0002, "Phase 2 Voltage", "V");
        add_field("phase3_voltage", 0x0004, "Phase 3 Voltage", "V");

        add_field("phase1_current", 0x0006, "Phase 1 Current", "A");
        add_field("phase2_current", 0x0008, "Phase 2 Current", "A");
        add_field("phase3_current", 0x000A, "Phase 3 Current", "A");

        add_field("phase1_active_power", 0x000C, "Phase 1 Active Power", "W");
        add_field("phase2_active_power", 0x000E, "Phase 2 Active Power", "W");
        add_field("phase3_active_power", 0x0010, "Phase 3 Active Power", "W");

        add_field("phase1_apparent_power",  0x0012, "Phase 1 Apparent Power", "W");
        add_field("phase2_apparent_power",  0x0014, "Phase 2 Apparent Power", "W");
        add_field("phase3_apparent_power",  0x0016, "Phase 3 Apparent Power", "W");

        add_field("phase1_reactive_power", 0x0018, "Phase 1 Reactive Power", "W");
        add_field("phase2_reactive_power", 0x001A, "Phase 2 Reactive Power", "W");
        add_field("phase3_reactive_power", 0x001C, "Phase 3 Reactive Power", "W");

        add_field("phase1_power_factor", 0x001E, "Phase 1 Power Factor", "");
        add_field("phase2_power_factor", 0x0020, "Phase 2 Power Factor", "");
        add_field("phase3_power_factor", 0x0022, "Phase 3 Power Factor", "");

        add_field("average_phase_voltage", 0x002A, "Average Phase Voltage", "V");
        add_field("average_phase_current", 0x002E, "Average Phase Current", "A");
        add_field("sum_of_phase_currents", 0x0030, "Sum of Phase Currents", "A");

        add_field("total_system_power",        0x0034, "Total System Power",        "W");
        add_field("total_system_volt_amps",    0x0038, "Total System Volt Amps",    "VA");
        add_field("total_system_var",          0x003C, "Total System VAr",          "VAr");
        add_field("total_system_power_factor", 0x003E, "Total System Power Factor", "");

        add_field("frequency",            0x0046, "Frequency",            "Hz");
        add_field("import_active_energy", 0x0048, "Import Active Energy", "kWh");
        add_field("export_active_energy", 0x004A, "Export Active Energy", "kWh");

        add_field("voltage_phase1_to_phase2",       0x00C8, "Voltage Phase 1 to 2",           "V");
        add_field("voltage_phase2_to_phase3",       0x00CA, "Voltage Phase 2 to 3",           "V");
        add_field("voltage_phase3_to_phase1",       0x00CC, "Voltage Phase 3 to 1",           "V");
        add_field("average_voltage_phase_to_phase", 0x00CE, "Average Phase to Phase Voltage", "V");

        add_field("neutral_current",       0x00E0, "Neutral Current",       "A");
        add_field("total_active_energy",   0x0156, "Total Active Energy",   "kWh");
        add_field("total_reactive_energy", 0x0158, "Total Reactive Energy", "kVArh");

        add_field("resettable_total_active_energy",         0x0180, "Resettable Total Active Energy",        "kWh");
        add_field("resettable_total_import_active_energy",  0x0184, "Resettable Total Reactive Energy",      "kWh");
        add_field("resettable_total_export_active_energy",  0x0186, "Resettable Total Export Active Energy", "kWh");

        add_field("net_kwh",                   0x018C, "Net Energy",                "kWh");
        add_field("total_import_active_power", 0x0500, "Total Import Active Power", "W");
        add_field("total_export_active_power", 0x0502, "Total Export Active Power", "W");
    }
};