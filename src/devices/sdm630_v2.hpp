#pragma once

#include "modbus_device_config.hpp"
#include "devices.hpp"

class Sdm630v2 : public ModbusDeviceConfig {
public:
    Sdm630v2() : ModbusDeviceConfig(RegisterType::Ireg) {
        max_chunk_size = 20;
    }

    void add_field(String name, uint16_t address, String description, String unit) {
        ModbusDeviceConfig::add_field(name, address, description, unit, 1.0f, FieldType::float32_reversed, true);
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

        add_field("phase1_apparent_power", 0x0012, "Phase 1 Apparent Power", "W");
        add_field("phase2_apparent_power", 0x0014, "Phase 2 Apparent Power", "W");
        add_field("phase3_apparent_power", 0x0016, "Phase 3 Apparent Power", "W");

        add_field("phase1_reactive_power", 0x0018, "Phase 1 Reactive Power", "W");
        add_field("phase2_reactive_power", 0x001A, "Phase 2 Reactive Power", "W");
        add_field("phase3_reactive_power", 0x001C, "Phase 3 Reactive Power", "W");

        add_field("phase1_power_factor", 0x001E, "Phase 1 Power Factor", "");
        add_field("phase2_power_factor", 0x0020, "Phase 2 Power Factor", "");
        add_field("phase3_power_factor", 0x0022, "Phase 3 Power Factor", "");

        add_field("phase1_angle", 0x0024, "Phase 1 Angle", "°");
        add_field("phase2_angle", 0x0026, "Phase 2 Angle", "°");
        add_field("phase3_angle", 0x0028, "Phase 3 Angle", "°");

        add_field("average_phase_voltage", 0x002A, "Average Phase Voltage", "V");
        add_field("average_phase_current", 0x002E, "Average Phase Current", "A");
        add_field("sum_of_phase_currents", 0x0030, "Sum of Phase Currents", "A");

        add_field("total_system_power",        0x0034, "Total System Power", "W");
        add_field("total_system_volt_amps",    0x0038, "Total System Volt Amps", "VA");
        add_field("total_system_var",          0x003C, "Total System VAr", "VAr");
        add_field("total_system_power_factor", 0x003E, "Total System Power Factor", "");

        add_field("total_system_phase_angle", 0x0042, "Total System Phase Angle", "°");

        add_field("frequency", 0x0046, "Frequency", "Hz");
        add_field("import_active_energy_since_last_reset", 0x0048, "Import Active Energy Since Last Reset", "kWh");
        add_field("export_active_energy_since_last_reset", 0x004A, "Export Active Energy Since Last Reset", "kWh");

        add_field("import_varh_since_last_reset", 0x004C, "Import VArh since last reset", "kVArh/MVArh");
        add_field("export_varh_since_last_reset", 0x004E, "Export VArh since last reset", "kVArh/MVArh");
        add_field("vah_since_last_reset",         0x0050, "VAh since last reset", "kVAh/MVAh");
        add_field("ah_since_last_reset",          0x0052, "Ah since last reset", "Ah/kAh");
        add_field("total_system_power_demand",    0x0054, "Total system power demand", "W");

        add_field("maximum_total_system_power_demand", 0x0056, "Maximum total system power demand", "VA");
        add_field("total_system_va_demand",            0x0064, "Total system VA demand", "VA");
        add_field("maximum_total_system_va_demand",    0x0066, "Maximum total system VA demand", "VA");
        add_field("neutral_current_demand",            0x0068, "Neutral current demand", "A");
        add_field("maximum_neutral_current_demand",    0x006A, "Maximum neutral current demand", "A");

        add_field("voltage_phase1_to_phase2",       0x00C8, "Voltage Phase 1 to 2", "V");
        add_field("voltage_phase2_to_phase3",       0x00CA, "Voltage Phase 2 to 3", "V");
        add_field("voltage_phase3_to_phase1",       0x00CC, "Voltage Phase 3 to 1", "V");
        add_field("average_voltage_phase_to_phase", 0x00CE, "Average Phase to Phase Voltage", "V");

        add_field("neutral_current", 0x00E0, "Neutral Current", "A");

        add_field("phase_1_ln_volts_thd", 0x00EA, "Phase 1 L/N volts THD", "%");
        add_field("phase_2_ln_volts_thd", 0x00EC, "Phase 2 L/N volts THD", "%");
        add_field("phase_3_ln_volts_thd", 0x00EE, "Phase 3 L/N volts THD", "%");

        add_field("phase_1_current_thd", 0x00F0, "Phase 1 Current THD", "%");
        add_field("phase_2_current_thd", 0x00F2, "Phase 2 Current THD", "%");
        add_field("phase_3_current_thd", 0x00F4, "Phase 3 Current THD", "%");

        add_field("avg_line_to_n_volts_thd",   0x00F8, "Average line to neutral volts THD", "%");
        add_field("avg_line_current_thd",      0x00FA, "Average line current THD", "%");
        add_field("total_system_power_factor", 0x00FE, "Total system power factor", "°");

        add_field("phase_1_current_demand", 0x0102, "Phase 1 current demand", "A");
        add_field("phase_2_current_demand", 0x0104, "Phase 2 current demand", "A");
        add_field("phase_3_current_demand", 0x0106, "Phase 3 current demand", "A");

        add_field("maximum_phase_1_current_demand", 0x0108, "Maximum phase 1 current demand", "A");
        add_field("maximum_phase_2_current_demand", 0x010A, "Maximum phase 2 current demand", "A");
        add_field("maximum_phase_3_current_demand", 0x010C, "Maximum phase 3 current demand", "A");

        add_field("line_1_to_line_2_volts_thd", 0x014E, "Line 1 to line 2 volts THD", "%");
        add_field("line_2_to_line_3_volts_thd", 0x0150, "Line 2 to line 3 volts THD", "%");
        add_field("line_3_to_line_1_volts_thd", 0x0152, "Line 3 to line 1 volts THD", "%");

        add_field("avg_line_to_line_volts_thd", 0x0154, "Average line to line volts THD", "%");

        add_field("total_active_energy_import_plus_export",   0x0156, "Total Active Energy (Import + Export)", "kWh");
        add_field("total_reactive_energy_import_plus_export", 0x0158, "Total Reactive Energy (import + Export)", "kVArh");

        add_field("l1_import_energy", 0x015A, "L1 import kWh", "kWh");
        add_field("l2_import_energy", 0x015C, "L2 import kWh", "kWh");
        add_field("l3_import_energy", 0x015E, "L3 import kWh", "kWh");
        add_field("l1_export_energy", 0x0160, "L1 export kWh", "kWh");
        add_field("l2_export_energy", 0x0162, "L2 export kWh", "kWh");
        add_field("l3_export_energy", 0x0164, "L3 export kWh", "kWh");
        add_field("l1_total_energy",  0x0166, "L1 total kWh", "kWh");
        add_field("l2_total_energy",  0x0168, "L2 total kWh", "kWh");
        add_field("l3_total_energy",  0x016A, "L3 total kWh", "kWh");

        add_field("l1_import_kvarh", 0x016C, "L1 import kVArh", "kVArh");
        add_field("l2_import_kvarh", 0x016E, "L2 import kVArh", "kVArh");
        add_field("l3_import_kvarh", 0x0170, "L3 import kVArh", "kVArh");
        add_field("l1_export_kvarh", 0x0172, "L1 export kVArh", "kVArh");
        add_field("l2_export_kvarh", 0x0174, "L2 export kVArh", "kVArh");
        add_field("l3_export_kvarh", 0x0176, "L3 export kVArh", "kVArh");
        add_field("l1_total_kvarh",  0x0178, "L1 total kVArh", "kVArh");
        add_field("l2_total_kvarh",  0x017A, "L2 total kVArh", "kVArh");
        add_field("l3_total_kvarh",  0x017C, "L3 total kVArh", "kVArh");

        add_field("net_kwh",   0x018C, "Net Energy (Import-Export)", "kWh");
        add_field("net_kvarh", 0x018E, "Net kVArh (Import-Export)", "kVArh");
    }
};
