#pragma once

#include "energy_meter_config.hpp"

#define NUM_FIELDS_V2 41

class Sdm72dmv2 : public EnergyMeterConfig {
public:
    Sdm72dmv2() : EnergyMeterConfig(NUM_FIELDS_V2, RegisterType::Ireg) {
        max_run_length = 20;
    }

    virtual void setup_registers() override {
        for(int i = 0; i < number_of_fields; i++) {
            field_data_type[i] = FieldType::float32_reversed;
            field_factor[i] = 1.0f;
            field_enabled[i] = true;
        }

        field_name[0] = "phase1_voltage"; field_address[0] = 0x0000; field_description[0] = "Phase 1 Voltage"; field_unit[0] = "V";
        field_name[1] = "phase2_voltage"; field_address[1] = 0x0002; field_description[1] = "Phase 2 Voltage"; field_unit[1] = "V";
        field_name[2] = "phase3_voltage"; field_address[2] = 0x0004; field_description[2] = "Phase 3 Voltage"; field_unit[2] = "V";

        field_name[3] = "phase1_current"; field_address[3] = 0x0006; field_description[3] = "Phase 1 Current"; field_unit[3] = "A";
        field_name[4] = "phase2_current"; field_address[4] = 0x0008; field_description[4] = "Phase 2 Current"; field_unit[4] = "A";
        field_name[5] = "phase3_current"; field_address[5] = 0x000A; field_description[5] = "Phase 3 Current"; field_unit[5] = "A";

        field_name[6] = "phase1_active_power"; field_address[6] = 0x000C; field_description[6] = "Phase 1 Active Power"; field_unit[6] = "W";
        field_name[7] = "phase2_active_power"; field_address[7] = 0x000E; field_description[7] = "Phase 2 Active Power"; field_unit[7] = "W";
        field_name[8] = "phase3_active_power"; field_address[8] = 0x0010; field_description[8] = "Phase 3 Active Power"; field_unit[8] = "W";

        field_name[9] = "phase1_apparent_power";  field_address[9] = 0x0012; field_description[9] = "Phase 1 Apparent Power"; field_unit[9] = "W";
        field_name[10] = "phase2_apparent_power"; field_address[10] = 0x0014; field_description[10] = "Phase 2 Apparent Power"; field_unit[10] = "W";
        field_name[11] = "phase3_apparent_power"; field_address[11] = 0x0016; field_description[11] = "Phase 3 Apparent Power"; field_unit[11] = "W";

        field_name[12] = "phase1_reactive_power"; field_address[12] = 0x0018; field_description[12] = "Phase 1 Reactive Power"; field_unit[12] = "W";
        field_name[13] = "phase2_reactive_power"; field_address[13] = 0x001A; field_description[13] = "Phase 2 Reactive Power"; field_unit[13] = "W";
        field_name[14] = "phase3_reactive_power"; field_address[14] = 0x001C; field_description[14] = "Phase 3 Reactive Power"; field_unit[14] = "W";

        field_name[15] = "phase1_power_factor"; field_address[15] = 0x001E; field_description[15] = "Phase 1 Power Factor"; field_unit[15] = "";
        field_name[16] = "phase2_power_factor"; field_address[16] = 0x0020; field_description[16] = "Phase 2 Power Factor"; field_unit[16] = "";
        field_name[17] = "phase3_power_factor"; field_address[17] = 0x0022; field_description[17] = "Phase 3 Power Factor"; field_unit[17] = "";

        field_name[18] = "average_phase_voltage"; field_address[18] = 0x002A; field_description[18] = "Average Phase Voltage"; field_unit[18] = "V";
        field_name[19] = "average_phase_current"; field_address[19] = 0x002E; field_description[19] = "Average Phase Current"; field_unit[19] = "A";
        field_name[20] = "sum_of_phase_currents"; field_address[20] = 0x0030; field_description[20] = "Sum of Phase Currents"; field_unit[20] = "A";

        field_name[21] = "total_system_power";        field_address[21] = 0x0034; field_description[21] = "Total System Power";        field_unit[21] = "W";
        field_name[22] = "total_system_volt_amps";    field_address[22] = 0x0038; field_description[22] = "Total System Volt Amps";    field_unit[22] = "VA";
        field_name[23] = "total_system_var";          field_address[23] = 0x003C; field_description[23] = "Total System VAr";          field_unit[23] = "VAr";
        field_name[24] = "total_system_power_factor"; field_address[24] = 0x003E; field_description[24] = "Total System Power Factor"; field_unit[24] = "";

        field_name[25] = "frequency";            field_address[25] = 0x0046; field_description[25] = "Frequency";            field_unit[25] = "Hz";
        field_name[26] = "import_active_energy"; field_address[26] = 0x0048; field_description[26] = "Import Active Energy"; field_unit[26] = "kWh";
        field_name[27] = "export_active_energy"; field_address[27] = 0x004A; field_description[27] = "Export Active Energy"; field_unit[27] = "kWh";

        field_name[28] = "voltage_phase1_to_phase2";       field_address[28] = 0x00C8; field_description[28] = "Voltage Phase 1 to 2"; field_unit[28] = "V";
        field_name[29] = "voltage_phase2_to_phase3";       field_address[29] = 0x00CA; field_description[29] = "Voltage Phase 2 to 3"; field_unit[29] = "V";
        field_name[30] = "voltage_phase3_to_phase1";       field_address[30] = 0x00CC; field_description[30] = "Voltage Phase 3 to 1"; field_unit[30] = "V";
        field_name[31] = "average_voltage_phase_to_phase"; field_address[31] = 0x00CE; field_description[31] = "Average Phase to Phase Voltage"; field_unit[31] = "V";

        field_name[32] = "neutral_current";       field_address[32] = 0x00E0; field_description[32] = "Neutral Current"; field_unit[32] = "A";
        field_name[33] = "total_active_energy";   field_address[33] = 0x0156; field_description[33] = "Total Active Energy"; field_unit[33] = "kWh";
        field_name[34] = "total_reactive_energy"; field_address[34] = 0x0158; field_description[34] = "Total Reactive Energy"; field_unit[34] = "kVArh";

        field_name[35] = "resettable_total_active_energy"; field_address[35] = 0x0180; field_description[35] = "Resettable Total Active Energy"; field_unit[35] = "kWh";
        field_name[36] = "resettable_total_import_active_energy"; field_address[36] = 0x0184; field_description[36] = "Resettable Total Reactive Energy"; field_unit[36] = "kWh";
        field_name[37] = "resettable_total_export_active_energy"; field_address[37] = 0x0186; field_description[37] = "Resettable Total Export Active Energy"; field_unit[37] = "kWh";

        field_name[38] = "net_kwh";                   field_address[38] = 0x018C; field_description[38] = "Net Energy";                field_unit[38] = "kWh";
        field_name[39] = "total_import_active_power"; field_address[39] = 0x0500; field_description[39] = "Total Import Active Power"; field_unit[39] = "W";
        field_name[40] = "total_export_active_power"; field_address[40] = 0x0502; field_description[40] = "Total Export Active Power"; field_unit[40] = "W";
    }
};

/*
constexpr uint8_t V2_PHASE1_VOLTAGE = 0;
constexpr uint8_t V2_PHASE2_VOLTAGE = 1;
constexpr uint8_t V2_PHASE3_VOLTAGE = 2;
constexpr uint8_t V2_PHASE1_CURRENT = 3;
constexpr uint8_t V2_PHASE2_CURRENT = 4;
constexpr uint8_t V2_PHASE3_CURRENT = 5;
constexpr uint8_t V2_PHASE1_ACTIVE_POWER = 6;
constexpr uint8_t V2_PHASE2_ACTIVE_POWER = 7;
constexpr uint8_t V2_PHASE3_ACTIVE_POWER = 8;
constexpr uint8_t V2_PHASE1_APPARENT_POWER = 9;
constexpr uint8_t V2_PHASE2_APPARENT_POWER = 10;
constexpr uint8_t V2_PHASE3_APPARENT_POWER = 11;
constexpr uint8_t V2_PHASE1_REACTIVE_POWERS = 12;
constexpr uint8_t V2_PHASE2_REACTIVE_POWER = 13;
constexpr uint8_t V2_PHASE3_REACTIVE_POWER = 14;
constexpr uint8_t V2_PHASE1_POWER_FACTOR = 15;
constexpr uint8_t V2_PHASE2_POWER_FACTOR = 16;
constexpr uint8_t V2_PHASE3_POWER_FACTOR = 17;
constexpr uint8_t V2_AVERAGE_PHASE_VOLTAGE = 18;
constexpr uint8_t V2_AVERAGE_PHASE_CURRENT = 19;
constexpr uint8_t V2_SUM_OF_PHASE_CURRENTS = 20;
constexpr uint8_t V2_TOTAL_SYSTEM_POWER = 21;
constexpr uint8_t V2_TOTAL_SYSTEM_VOLT_AMPS = 22;
constexpr uint8_t V2_TOTAL_SYSTEM_VAR = 23;
constexpr uint8_t V2_TOTAL_SYSTEM_POWER_FACTOR = 24;
constexpr uint8_t V2_FREQUENCY_OF_SUPPLY_VOLTAGES = 25;
constexpr uint8_t V2_IMPORT_ACTIVE_ENERGY = 26;
constexpr uint8_t V2_EXPORT_ACTIVE_ENERGY = 27;
constexpr uint8_t V2_VOLTAGE_PHASE1_TO_PHASE2 = 28;
constexpr uint8_t V2_VOLTAGE_PHASE2_TO_PHASE3 = 29;
constexpr uint8_t V2_VOLTAGE_PHASE3_TO_PHASE1 = 30;
constexpr uint8_t V2_AVERAGE_VOLTAGE_PHASE_TO_PHASE = 31;
constexpr uint8_t V2_NEUTRAL_CURRENT = 32;
constexpr uint8_t V2_TOTAL_ACTIVE_ENERGY = 33;
constexpr uint8_t V2_TOTAL_REACTIVE_ENERGY = 34;
constexpr uint8_t V2_RESETTABLE_TOTAL_ACTIVE_ENERGY = 35;
constexpr uint8_t V2_RESETTABLE_TOTAL_IMPORT_ACTIVE_ENERGY = 36;
constexpr uint8_t V2_RESETTABLE_TOTAL_EXPORT_ACTIVE_ENERGY = 37;
constexpr uint8_t V2_NET_KWH = 38;
constexpr uint8_t V2_TOTAL_IMPORT_ACTIVE_POWER = 39;
constexpr uint8_t V2_TOTAL_EXPORT_ACTIVE_POWER = 40;
*/