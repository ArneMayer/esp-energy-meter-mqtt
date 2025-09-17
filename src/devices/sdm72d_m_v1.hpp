#pragma once

#include "modbus/modbus_device.hpp"
#include "modbus/modbus_connection.hpp"

class Sdm72dmv1 : public ModbusDevice {
public:
    Sdm72dmv1(std::shared_ptr<ModbusConnection> connection, uint16_t modbus_id) : ModbusDevice(connection, modbus_id, RegisterType::Ireg, {
        Field("total_power",            0x0034, "Total Power",            "W",   1.0f, FieldType::float32_reversed),
        Field("total_power",            0x0034, "Total Power",            "W",   1.0f, FieldType::float32_reversed),
        Field("import_since_reset",     0x0048, "Import Since Reset",     "kWh", 1.0f, FieldType::float32_reversed),
        Field("export_since_reset",     0x004A, "Export Since Reset",     "kWh", 1.0f, FieldType::float32_reversed),
        Field("total_energy",           0x0156, "Total Energy",           "kWh", 1.0f, FieldType::float32_reversed),
        Field("settable_total_energy",  0x0180, "Settable Total Energy",  "kWh", 1.0f, FieldType::float32_reversed),
        Field("settable_import_energy", 0x0184, "Settable Import Energy", "kWh", 1.0f, FieldType::float32_reversed),
        Field("settable_export_energy", 0x0186, "Settable Export Energy", "kWh", 1.0f, FieldType::float32_reversed),
        Field("import_power",           0x0500, "Import Power",           "W",   1.0f, FieldType::float32_reversed),
        Field("export_power",           0x0502, "Export Power",           "W",   1.0f, FieldType::float32_reversed)
    }, 20){
    }
};