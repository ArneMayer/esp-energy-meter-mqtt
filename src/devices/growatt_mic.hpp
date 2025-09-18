#pragma once

#include "modbus/modbus_device.hpp"
#include "modbus/modbus_connection.hpp"

class GrowattMic : public ModbusDevice {
private:

    static Field ireg(const char* name, uint16_t address, const char* description, const char* unit, float factor, DataType type) {
        return Field(name, address, description, unit, factor, type, RegisterType::Ireg);
    }

public:
    GrowattMic(std::shared_ptr<ModbusConnection> connection, ModbusId modbus_id) : ModbusDevice(connection, modbus_id, std::vector({
            ireg("inverter_status", 0, "Inverter run state (0:waiting, 1:normal, 3:fault)", "", 1.0f, DataType::uint16),
            ireg("input_power", 1, "Input Power", "W", 0.1f, DataType::uint32),
            ireg("pv1_voltage", 3, "PV1 voltage", "V", 0.1f, DataType::uint16),
            ireg("pv1_current", 4, "PV1 input current", "A", 0.1f, DataType::uint16),
            ireg("pv1_input_power", 5, "PV1 input power (high)", "W", 0.1f, DataType::uint32),

            ireg("output_power", 35, "Output power", "W", 0.1f, DataType::uint32),
            ireg("grid_frequency", 37, "Grid frequency", "Hz", 0.01f, DataType::uint16),
            ireg("grid_voltage", 38, "Three/single phase grid voltage", "V", 0.1f, DataType::uint16),
            ireg("grid_current", 39, "Three/single phase grid output current", "A", 0.1f, DataType::uint16),
            ireg("grid_output_power", 40, "Three/single phase grid output power", "VA", 0.1f, DataType::uint32),

            ireg("energy_today", 53, "Today generated energy", "kWh", 0.1f, DataType::uint32),
            ireg("energy_total", 55, "Total generated energy", "kWh", 0.1f, DataType::uint32),
            ireg("work_time_total", 57, "Work time total", "s", 0.5f, DataType::uint32),
            ireg("pv1_energy_today", 59, "PV1 energy today", "kWh", 0.1f, DataType::uint32),

            ireg("temperature_inverter", 93, "Inverter temperature", "°C", 0.1f, DataType::uint16),
            ireg("temperature_ipm", 94, "Inside IPM temperature", "°C", 0.1f, DataType::uint16),
            ireg("temperature_boost", 95, "Boost temperature", "°C", 0.1f, DataType::uint16),

            ireg("p_bus_voltage", 98, "P Bus inside voltage", "V", 0.1f, DataType::uint16),
            ireg("n_bus_voltage", 99, "N Bus inside voltage", "V", 0.1f, DataType::uint16),

            ireg("fault_code", 105, "Inverter fault code", "", 1.0f, DataType::uint16),
            ireg("fault_bitcode", 106, "Inverter fault bitcode", "", 1.0f, DataType::uint32)}), 20)
        {}
};