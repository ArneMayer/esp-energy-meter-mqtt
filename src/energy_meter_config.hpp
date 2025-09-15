#pragma once

#include <WString.h>
#include <stdint.h>

#include "logging.hpp"

class Chunk {
public:
    uint16_t start_address;
    uint16_t number_of_words;
    uint16_t buffer_position;

    Chunk(uint16_t start, uint16_t word_count, uint16_t buffer_position) {
        start_address = start;
        number_of_words = word_count;
        buffer_position = buffer_position;
    }
};

struct Field {
    String name;
    uint16_t address;
    String description;
    String unit;
    float factor;
    FieldType type;
    uint16_t buffer_position;
    bool enabled;

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
};

class EnergyMeterConfig {
public:
    uint8_t number_of_fields;
    RegisterType register_type;
    size_t buffer_size;
    size_t max_chunk_size = 20;
    bool auto_config = true;
    
    std::vector<Field> fields;
    std::vector<Chunk> chunks;

    EnergyMeterConfig(RegisterType reg_type) {
        register_type = reg_type;
    }

    void add_field(String name, uint16_t address, String description, String unit, float factor, FieldType type, bool enabled = true) {
        fields.push_back({name, address, description, unit, factor, type, enabled});
    }

    size_t setup_buffer_map() {
        size_t buffer_position = 0;
        for(int i = 0; i < number_of_fields; i++) {
            if (fields[i].enabled) {
                fields[i].buffer_position = buffer_position;
                if (i >= number_of_fields - 1 || fields[i+1].address != fields[i].address) {
                    buffer_position += fields[i].length();
                }
            }
        }

        return buffer_position;
    }

    void setup_chunks() {
        std::sort(fields.begin(), fields.end(), [](const Field& a, const Field& b){
            return a.address < b.address;
        });

        if (fields.empty()) {
            return;
        }

        Chunk chunk(fields[0].address, fields[0].length(), fields[0].buffer_position);

        for (int i = 1; i < fields.size(); i++) {
            const auto& field = fields[i];

            // Extend Chunk
            if (field.address <= chunk.start_address + chunk.number_of_words && chunk.number_of_words + field.length() < max_chunk_size) {
                chunk.number_of_words = field.address + field.length() - chunk.start_address;
            }
            // Finish Chunk
            else {
                chunks.push_back(chunk);
                chunk = Chunk(field.address, field.length(), field.buffer_position);
            }
        }

        // Finish last chunk
        chunks.push_back(chunk);
    }

    void setup() {
        setup_registers();
        if (auto_config) {
            buffer_size = setup_buffer_map();
            setup_chunks();
        }
    }

    virtual void setup_registers() = 0;
};