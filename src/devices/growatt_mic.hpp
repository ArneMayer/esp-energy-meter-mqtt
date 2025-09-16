#pragma once

#include "modbus_device.hpp"
#include "devices.hpp"
#include "field.hpp"

class GrowattMic : public ModbusDevice {
public:
    GrowattMic(std::shared_ptr<ModbusConnection> connection, uint16_t modbus_id) : ModbusDevice(connection, modbus_id, RegisterType::Ireg, std::vector({
            Field("inverter_status", 0, "Inverter run state (0:waiting, 1:normal, 3:fault)", "", 1.0f, FieldType::uint16),
            Field("input_power", 1, "Input Power", "W", 0.1f, FieldType::uint32),
            Field("pv1_voltage", 3, "PV1 voltage", "V", 0.1f, FieldType::uint16),
            Field("pv1_current", 4, "PV1 input current", "A", 0.1f, FieldType::uint16),
            Field("pv1_input_power", 5, "PV1 input power (high)", "W", 0.1f, FieldType::uint32),

            Field("output_power", 35, "Output power", "W", 0.1f, FieldType::uint32),
            Field("grid_frequency", 37, "Grid frequency", "Hz", 0.01f, FieldType::uint16),
            Field("grid_voltage", 38, "Three/single phase grid voltage", "V", 0.1f, FieldType::uint16),
            Field("grid_current", 39, "Three/single phase grid output current", "A", 0.1f, FieldType::uint16),
            Field("grid_output_power", 40, "Three/single phase grid output power", "VA", 0.1f, FieldType::uint32),

            Field("energy_today", 53, "Today generated energy", "kWh", 0.1f, FieldType::uint32),
            Field("energy_total", 55, "Total generated energy", "kWh", 0.1f, FieldType::uint32),
            Field("work_time_total", 57, "Work time total", "s", 0.5f, FieldType::uint32),

            Field("pv1_energy_today", 59, "PV1 energy today", "kWh", 0.1f, FieldType::uint32),

            Field("temperature_inverter", 93, "Inverter temperature", "°C", 0.1f, FieldType::uint16),
            Field("temperature_ipm", 94, "Inside IPM temperature", "°C", 0.1f, FieldType::uint16),
            Field("temperature_boost", 95, "Boost temperature", "°C", 0.1f, FieldType::uint16),

            Field("p_bus_voltage", 98, "P Bus inside voltage", "V", 0.1f, FieldType::uint16),
            Field("n_bus_voltage", 99, "N Bus inside voltage", "V", 0.1f, FieldType::uint16),

            Field("fault_code", 105, "Inverter fault code", "", 1.0f, FieldType::uint16),
            Field("fault_bitcode", 106, "Inverter fault bitcode", "", 1.0f, FieldType::uint32)}), 20)
        {}
};