#pragma once

#include "modbus/modbus_device.hpp"
#include "modbus/modbus_connection.hpp"

class Dts238_7 : public ModbusDevice {
private:
    static Field hreg(const char* name, uint16_t address, const char* description, const char* unit, float factor, DataType type) {
        return Field(name, address, description, unit, factor, type, RegisterType::Hreg);
    }

public:
    Dts238_7(std::shared_ptr<ModbusConnection> connection, ModbusId modbus_id) : ModbusDevice(connection, modbus_id, {
        hreg("total_energy_count",        0x00, "Total Energy Count",        "kWh", 0.01f,  DataType::uint32),
        hreg("total_export_energy_count", 0x08, "Total Export Energy Count", "kWh", 0.01f,  DataType::uint32),
        hreg("total_import_energy_count", 0x0A, "Total Import Energy Count", "kWh", 0.01f,  DataType::uint32),
        hreg("frequency",                 0x11, "Frequency",                 "Hz",  0.01f,  DataType::uint16),
        hreg("communication_address",     0x15, "Communication address",     "",    1.0f,   DataType::uint8_high_byte),
        hreg("baudrate",                  0x15, "Baudrate",                  "",    1.0f,   DataType::uint8_low_byte),
        hreg("voltage_l1",                0x80, "Voltage L1",                "V",   0.1f,   DataType::uint16),
        hreg("voltage_l2",                0x81, "Voltage L2",                "V",   0.1f,   DataType::uint16),
        hreg("voltage_l3",                0x82, "Voltage L3",                "V",   0.1f,   DataType::uint16),
        hreg("current_l1",                0x83, "Current L1",                "A",   0.01f,  DataType::uint16),
        hreg("current_l2",                0x84, "Current L2",                "A",   0.01f,  DataType::uint16),
        hreg("current_l3",                0x85, "Current L3",                "A",   0.01f,  DataType::uint16),
        hreg("total_active_power",        0x86, "Total Active Power",        "W",   1.0f,   DataType::int32),
        hreg("active_power_l1",           0x88, "Active Power L1",           "W",   1.0f,   DataType::int16),
        hreg("active_power_l2",           0x89, "Active Power L2",           "W",   1.0f,   DataType::int16),
        hreg("active_power_l3",           0x8A, "Active Power L3",           "W",   1.0f,   DataType::int16),
        hreg("total_reactive_power",      0x8B, "Total Reactive Power",      "W",   1.0f,   DataType::int32),
        hreg("reactive_power_l1",         0x8D, "Reactive Power L1",         "W",   1.0f,   DataType::int16),
        hreg("reactive_power_l2",         0x8E, "Reactive Power L2",         "W",   1.0f,   DataType::int16),
        hreg("reactive_power_l3",         0x8F, "Reactive Power L3",         "W",   1.0f,   DataType::int16),
        hreg("total_apparent_power",      0x90, "Total Apparent Power",      "W",   1.0f,   DataType::uint32),
        hreg("apparent_power_l1",         0x92, "Apparent Power L1",         "W",   1.0f,   DataType::uint16),
        hreg("apparent_power_l2",         0x93, "Apparent Power L2",         "W",   1.0f,   DataType::uint16),
        hreg("apparent_power_l3",         0x94, "Apparent Power L3",         "W",   1.0f,   DataType::uint16),
        hreg("total_power_factor",        0x95, "Total Power Factor",        "",    0.001f, DataType::uint16),
        hreg("power_factor_l1",           0x96, "Power Factor L1",           "",    0.001f, DataType::uint16),
        hreg("power_factor_l2",           0x97, "Power Factor L2",           "",    0.001f, DataType::uint16),
        hreg("power_factor_l3",           0x98, "Power Factor L3",           "",    0.001f, DataType::uint16)},
        25) {

        _buffer.resize(22 + 25); 
        _buffer_positions.clear();

        for (int i = 0; i <= 5; i++) {
            _buffer_positions[i] = _fields[i].address;
        }

        for (int i = 6; i <= 27; i++) {
            _buffer_positions[i] = _fields[i].address - 0x80 + 22;
        }

        _chunks = {Chunk(0x00, 22, 0, RegisterType::Hreg), Chunk(0x80, 25, 22, RegisterType::Hreg)};
    }
};