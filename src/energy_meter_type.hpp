#pragma once

enum class EnergyMeterType {
    SDM72D_M_V1,
    SDM72D_M_V2,
    DTS238_7
};

enum class FieldType {
    float32,
    float32_reversed,
    uint8_low_byte,
    uint8_high_byte,
    uint16,
    uint32,
    int16,
    int32

};

enum class RegisterType {
    Ireg,
    Hreg
};