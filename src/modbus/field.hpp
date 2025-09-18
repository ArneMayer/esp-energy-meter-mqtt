#pragma once

enum class DataType {
    float32,
    float32_reversed,
    uint8_low_byte,
    uint8_high_byte,
    uint16,
    uint32,
    int16,
    int32
};

struct Field {
    const char* name;
    uint16_t address;
    const char* description;
    const char* unit;
    float factor;
    DataType type;
    RegisterType register_type;
    bool enabled;

    Field(const char* name, uint16_t address, const char* description, const char* unit, float factor, DataType type, RegisterType register_type, bool enabled = true)
        : name(name),
        address(address),
        description(description),
        unit(unit),
        factor(factor),
        type(type),
        register_type(register_type),
        enabled(enabled) {}

    uint16_t length() const {
        switch (type) {
            case DataType::float32:
            case DataType::float32_reversed:
            case DataType::int32:
            case DataType::uint32:
                return 2;

            case DataType::int16:
            case DataType::uint16:
            case DataType::uint8_high_byte:
            case DataType::uint8_low_byte:
                return 1;

            default:
                return 0;
        }
    }

    bool operator<(const Field& rhs) const {
        if (name == nullptr) return rhs.name != nullptr; // nullptr < anything else
        if (rhs.name == nullptr) return false;          // anything else > nullptr
        return strcmp(name, rhs.name) < 0;
    }
};