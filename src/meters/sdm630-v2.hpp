#pragma once

#include "energy_meter_config.hpp"

#define NUM_FIELDS_SDM630 88

class Sdm630v2 : public EnergyMeterConfig {
public:
    Sdm630v2() : EnergyMeterConfig(NUM_FIELDS_SDM630, RegisterType::Ireg) {
        max_run_length = 20;
    }

    virtual void setup_registers() override {
        for(int i = 0; i < number_of_fields; i++) {
            field_data_type[i] = FieldType::float32_reversed;
            field_factor[i] = 1.0f;
            field_enabled[i] = true;
        }

        size_t i = 0;
        field_name[i] = "phase1_voltage"; field_address[i] = 0x0000; field_description[i] = "Phase 1 Voltage"; field_unit[i] = "V"; i++;
        field_name[i] = "phase2_voltage"; field_address[i] = 0x0002; field_description[i] = "Phase 2 Voltage"; field_unit[i] = "V"; i++;
        field_name[i] = "phase3_voltage"; field_address[i] = 0x0004; field_description[i] = "Phase 3 Voltage"; field_unit[i] = "V"; i++;

        field_name[i] = "phase1_current"; field_address[i] = 0x0006; field_description[i] = "Phase 1 Current"; field_unit[i] = "A"; i++;
        field_name[i] = "phase2_current"; field_address[i] = 0x0008; field_description[i] = "Phase 2 Current"; field_unit[i] = "A"; i++;
        field_name[i] = "phase3_current"; field_address[i] = 0x000A; field_description[i] = "Phase 3 Current"; field_unit[i] = "A"; i++;

        field_name[i] = "phase1_active_power"; field_address[i] = 0x000C; field_description[i] = "Phase 1 Active Power"; field_unit[i] = "W"; i++;
        field_name[i] = "phase2_active_power"; field_address[i] = 0x000E; field_description[i] = "Phase 2 Active Power"; field_unit[i] = "W"; i++;
        field_name[i] = "phase3_active_power"; field_address[i] = 0x0010; field_description[i] = "Phase 3 Active Power"; field_unit[i] = "W"; i++;

        field_name[i] = "phase1_apparent_power";  field_address[i] = 0x0012; field_description[i] = "Phase 1 Apparent Power"; field_unit[i] = "W"; i++;
        field_name[i] = "phase2_apparent_power"; field_address[i] = 0x0014; field_description[i] = "Phase 2 Apparent Power"; field_unit[i] = "W"; i++;
        field_name[i] = "phase3_apparent_power"; field_address[i] = 0x0016; field_description[i] = "Phase 3 Apparent Power"; field_unit[i] = "W"; i++;

        field_name[i] = "phase1_reactive_power"; field_address[i] = 0x0018; field_description[i] = "Phase 1 Reactive Power"; field_unit[i] = "W"; i++;
        field_name[i] = "phase2_reactive_power"; field_address[i] = 0x001A; field_description[i] = "Phase 2 Reactive Power"; field_unit[i] = "W"; i++;
        field_name[i] = "phase3_reactive_power"; field_address[i] = 0x001C; field_description[i] = "Phase 3 Reactive Power"; field_unit[i] = "W"; i++;
        field_name[i] = "phase1_power_factor"; field_address[i] = 0x001E; field_description[i] = "Phase 1 Power Factor"; field_unit[i] = ""; i++;
        field_name[i] = "phase2_power_factor"; field_address[i] = 0x0020; field_description[i] = "Phase 2 Power Factor"; field_unit[i] = ""; i++;
        field_name[i] = "phase3_power_factor"; field_address[i] = 0x0022; field_description[i] = "Phase 3 Power Factor"; field_unit[i] = ""; i++;

        field_name[i] = "phase1_angle"; field_address[i] = 0x0024; field_description[i] = "Phase 1 Angle"; field_unit[i] = "°"; i++;
        field_name[i] = "phase2_angle"; field_address[i] = 0x0026; field_description[i] = "Phase 2 Angle"; field_unit[i] = "°"; i++;
        field_name[i] = "phase3_angle"; field_address[i] = 0x0028; field_description[i] = "Phase 3 Angle"; field_unit[i] = "°"; i++;
        field_name[i] = "average_phase_voltage"; field_address[i] = 0x002A; field_description[i] = "Average Phase Voltage"; field_unit[i] = "V"; i++;
        field_name[i] = "average_phase_current"; field_address[i] = 0x002E; field_description[i] = "Average Phase Current"; field_unit[i] = "A"; i++;
        field_name[i] = "sum_of_phase_currents"; field_address[i] = 0x0030; field_description[i] = "Sum of Phase Currents"; field_unit[i] = "A"; i++;

        field_name[i] = "total_system_power";        field_address[i] = 0x0034; field_description[i] = "Total System Power";        field_unit[i] = "W"; i++;
        field_name[i] = "total_system_volt_amps";    field_address[i] = 0x0038; field_description[i] = "Total System Volt Amps";    field_unit[i] = "VA"; i++;
        field_name[i] = "total_system_var";          field_address[i] = 0x003C; field_description[i] = "Total System VAr";          field_unit[i] = "VAr"; i++;
        field_name[i] = "total_system_power_factor"; field_address[i] = 0x003E; field_description[i] = "Total System Power Factor"; field_unit[i] = ""; i++;

        field_name[i] = "total_system_phase_angle"; field_address[i] = 0x0042; field_description[i] = "Total System Phase Angle"; field_unit[i] = "°"; i++;

        field_name[i] = "frequency";            field_address[i] = 0x0046; field_description[i] = "Frequency";            field_unit[i] = "Hz"; i++;
        field_name[i] = "import_active_energy"; field_address[i] = 0x0048; field_description[i] = "Import Active Energy"; field_unit[i] = "kWh"; i++;
        field_name[i] = "export_active_energy"; field_address[i] = 0x004A; field_description[i] = "Export Active Energy"; field_unit[i] = "kWh"; i++;

        field_name[i] = "import_varh_since_last_reset"; field_address[i] = 0x004C; field_description[i] = "Import VArh since last reset"; field_unit[i] = "kVArh/MVArh"; i++;
        field_name[i] = "export_varh_since_last_reset"; field_address[i] = 0x004E; field_description[i] = "Export VArh since last reset"; field_unit[i] = "kVArh/MVArh"; i++;
        field_name[i] = "vah_since_last_reset";         field_address[i] = 0x0050; field_description[i] = "VAh since last reset";         field_unit[i] = "kVAh/MVAh"; i++;
        field_name[i] = "ah_since_last_reset";          field_address[i] = 0x0052; field_description[i] = "Ah since last reset";          field_unit[i] = "Ah/kAh"; i++;
        field_name[i] = "total_system_power_demand";    field_address[i] = 0x0054; field_description[i] = "Total system power demand";    field_unit[i] = "W"; i++;

        field_name[i] = "maximum_total_system_power_demand"; field_address[i] = 0x0056; field_description[i] = "Maximum total system power demand"; field_unit[i] = "VA"; i++;
        field_name[i] = "total_system_va_demand";            field_address[i] = 0x0064; field_description[i] = "Total system VA demand";            field_unit[i] = "VA"; i++;
        field_name[i] = "maximum_total_system_va_demand";    field_address[i] = 0x0066; field_description[i] = "Maximum total system VA demand";    field_unit[i] = "VA"; i++;
        field_name[i] = "neutral_current_demand";            field_address[i] = 0x0068; field_description[i] = "Neutral current demand";            field_unit[i] = "A"; i++;
        field_name[i] = "maximum_neutral_current_demand";    field_address[i] = 0x006A; field_description[i] = "Maximum neutral current demand";    field_unit[i] = "A"; i++;

        field_name[i] = "voltage_phase1_to_phase2";       field_address[i] = 0x00C8; field_description[i] = "Voltage Phase 1 to 2"; field_unit[i] = "V"; i++;
        field_name[i] = "voltage_phase2_to_phase3";       field_address[i] = 0x00CA; field_description[i] = "Voltage Phase 2 to 3"; field_unit[i] = "V"; i++;
        field_name[i] = "voltage_phase3_to_phase1";       field_address[i] = 0x00CC; field_description[i] = "Voltage Phase 3 to 1"; field_unit[i] = "V"; i++;
        field_name[i] = "average_voltage_phase_to_phase"; field_address[i] = 0x00CE; field_description[i] = "Average Phase to Phase Voltage"; field_unit[i] = "V"; i++;

        field_name[i] = "neutral_current";       field_address[i] = 0x00E0; field_description[i] = "Neutral Current"; field_description[i] = "A"; i++;
      
        field_name[i] = "phase_1_ln_volts_thd"; field_address[i] = 0x00EA; field_description[i] = "Phase 1 L/N volts THD"; field_description[i] = "%"; i++;
        field_name[i] = "phase_2_ln_volts_thd"; field_address[i] = 0x00EC; field_description[i] = "Phase 2 L/N volts THD"; field_description[i] = "%"; i++;
        field_name[i] = "phase_3_ln_volts_thd"; field_address[i] = 0x00EE; field_description[i] = "Phase 3 L/N volts THD"; field_description[i] = "%"; i++;

        field_name[i] = "phase_1_current_thd"; field_address[i] = 0x00F0; field_description[i] = "Phase 1 Current THD"; field_description[i] = "%"; i++;
        field_name[i] = "phase_2_current_thd"; field_address[i] = 0x00F2; field_description[i] = "Phase 2 Current THD"; field_description[i] = "%"; i++;
        field_name[i] = "phase_3_current_thd"; field_address[i] = 0x00F4; field_description[i] = "Phase 3 Current THD"; field_description[i] = "%"; i++;

        field_name[i] = "avg_line_to_n_volts_thd";   field_address[i] = 0x00F8; field_description[i] = "Average line to neutral volts THD"; field_description[i] = "%"; i++;
        field_name[i] = "avg_line_current_thd";      field_address[i] = 0x00FA; field_description[i] = "Average line current THD"; field_description[i] = "%"; i++;
        field_name[i] = "total_system_power_factor"; field_address[i] = 0x00FE; field_description[i] = "Total system power factor"; field_description[i] = "°"; i++;

        field_name[i] = "phase_1_current_demand"; field_address[i] = 0x0102; field_description[i] = "Phase 1 current demand"; field_description[i] = "A"; i++;
        field_name[i] = "phase_2_current_demand"; field_address[i] = 0x0104; field_description[i] = "Phase 2 current demand"; field_description[i] = "A"; i++;
        field_name[i] = "phase_3_current_demand"; field_address[i] = 0x0106; field_description[i] = "Phase 3 current demand"; field_description[i] = "A"; i++;

        field_name[i] = "maximum_phase_1_current_demand"; field_address[i] = 0x0108; field_description[i] = "Maximum phase 1 current demand"; field_description[i] = "A"; i++;
        field_name[i] = "maximum_phase_2_current_demand"; field_address[i] = 0x010A; field_description[i] = "Maximum phase 2 current demand"; field_description[i] = "A"; i++;
        field_name[i] = "maximum_phase_3_current_demand"; field_address[i] = 0x010C; field_description[i] = "Maximum phase 3 current demand"; field_description[i] = "A"; i++;

        field_name[i] = "line_1_to_line_2_volts_thd"; field_address[i] = 0x014E; field_description[i] = "Line 1 to line 2 volts THD"; field_description[i] = "%"; i++;
        field_name[i] = "line_2_to_line_3_volts_thd"; field_address[i] = 0x0150; field_description[i] = "Line 2 to line 3 volts THD"; field_description[i] = "%"; i++;
        field_name[i] = "line_3_to_line_1_volts_thd"; field_address[i] = 0x0152; field_description[i] = "Line 3 to line 1 volts THD"; field_description[i] = "%"; i++;

        field_name[i] = "avg_line_to_line_volts_thd"; field_address[i] = 0x0154; field_description[i] = "Average line to line volts THD"; field_description[i] = "%"; i++;
  
        field_name[i] = "total_active_energy";   field_address[i] = 0x0156; field_description[i] = "Total Active Energy"; field_unit[i] = "kWh"; i++;
        field_name[i] = "total_reactive_energy"; field_address[i] = 0x0158; field_description[i] = "Total Reactive Energy"; field_unit[i] = "kVArh"; i++;

        field_name[i] = "l1_import_energy"; field_address[i] = 0x015a; field_description[i] = "L1 import kwh"; field_unit[i] = "kwh"; i++;
        field_name[i] = "l2_import_energy"; field_address[i] = 0x015c; field_description[i] = "L2 import kwh"; field_unit[i] = "kwh"; i++;
        field_name[i] = "l3_import_energy"; field_address[i] = 0x015e; field_description[i] = "L3 import kWh"; field_unit[i] = "kwh"; i++;
        field_name[i] = "l1_export_energy"; field_address[i] = 0x0160; field_description[i] = "L1 export kWh"; field_unit[i] = "kwh"; i++;
        field_name[i] = "l2_export_energy"; field_address[i] = 0x0162; field_description[i] = "L2 export kwh"; field_unit[i] = "kwh"; i++;
        field_name[i] = "l3_export_energy"; field_address[i] = 0x0164; field_description[i] = "L3 export kWh"; field_unit[i] = "kwh"; i++;
        field_name[i] = "l1_total_energy"; field_address[i] = 0x0166; field_description[i] = "L1 total kwh"; field_unit[i] = "kwh"; i++;
        field_name[i] = "l2_total_energy"; field_address[i] = 0x0168; field_description[i] = "L2 total kWh"; field_unit[i] = "kwh"; i++;
        field_name[i] = "l3_total_energy"; field_address[i] = 0x016a; field_description[i] = "L3 total kwh"; field_unit[i] = "kwh"; i++;
        field_name[i] = "l1_import_kvarh"; field_address[i] = 0x016c; field_description[i] = "L1 import kvarh"; field_unit[i] = "kvarh"; i++;
        field_name[i] = "l2_import_kvarh"; field_address[i] = 0x016e; field_description[i] = "L2 import kvarh"; field_unit[i] = "kvarh"; i++;
        field_name[i] = "l3_import_kvarh"; field_address[i] = 0x0170; field_description[i] = "L3 import kvarh"; field_unit[i] = "kvarh"; i++;
        field_name[i] = "l1_export_kvarh"; field_address[i] = 0x0172; field_description[i] = "L1 export kvarh"; field_unit[i] = "kvarh"; i++;
        field_name[i] = "l2_export_kvarh"; field_address[i] = 0x0174; field_description[i] = "L2 export kvarh"; field_unit[i] = "kvarh"; i++;
        field_name[i] = "l3_export_kvarh"; field_address[i] = 0x0176; field_description[i] = "L3 export kvarh"; field_unit[i] = "kvarh"; i++;
        field_name[i] = "l1_total_kvarh"; field_address[i] = 0x0178; field_description[i] = "L1 total kvarh"; field_unit[i] = "kvarh"; i++;
        field_name[i] = "l2_total_kvarh"; field_address[i] = 0x017a; field_description[i] = "L2 total kvarh"; field_unit[i] = "kvarh"; i++;
        field_name[i] = "l3_total_kvarh"; field_address[i] = 0x017c; field_description[i] = "L3 total kvarh"; field_unit[i] = "kvarh"; i++;

        field_name[i] = "net_kwh";   field_address[i] = 0x018C; field_description[i] = "Net Energy";                field_unit[i] = "kWh"; i++;
        field_name[i] = "net_kvarh"; field_address[i] = 0x018e; field_description[i] = "Net kVArh (Import-Export)"; field_unit[i] = "kvarh"; i++;
    }
};
