#pragma once

#include <WString.h>

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

struct Field {
    const char* name;
    uint16_t address;
    const char* description;
    const char* unit;
    float factor;
    FieldType type;
    bool enabled;

    Field(const char* name, uint16_t address, const char* description, const char* unit, float factor, FieldType type, bool enabled = true)
        : name(name),
        address(address),
        description(description),
        unit(unit),
        factor(factor),
        type(type),
        enabled(enabled) {}

    size_t length() const {
        switch (type) {
            case FieldType::float32:
            case FieldType::float32_reversed:
            case FieldType::int32:
            case FieldType::uint32:
                return 2;

            case FieldType::int16:
            case FieldType::uint16:
            case FieldType::uint8_high_byte:
            case FieldType::uint8_low_byte:
                return 1;

            default:
                return 0;
        }
    }

    bool operator< (const Field& rhs) const {
        return name < rhs.name;
    }
};