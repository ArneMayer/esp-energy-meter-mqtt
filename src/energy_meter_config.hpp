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
        if (type == FieldType::float32) {
            return 2;
        } else if (type == FieldType::float32_reversed) {
            return 2;
        } else if (type == FieldType::int16) {
            return 1;
        } else if (type == FieldType::int32) {
            return 2;
        } else if (type == FieldType::uint16) {
            return 1;
        } else if (type == FieldType::uint32) {
            return 2;
        } else if (type == FieldType::uint8_high_byte) {
            return 1;
        } else if (type == FieldType::uint8_low_byte) {
            return 1;
        } else {
            return 0;
        }
    }
};

class EnergyMeterConfig {
public:
    uint8_t number_of_fields;
    RegisterType register_type;
    size_t buffer_size;
    size_t max_run_length = 20;
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
                debug_print("field_data_map["); debug_print(i); debug_print("] = "); debug_println(buffer_position);
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

        uint16_t current_index = 1;
        bool at_the_end = false;
        uint16_t chunk_start_index = 0;
        uint16_t chunk_length_words = fields[0].length();

        while(!at_the_end) {
            const auto& field = fields[current_index];
            const auto& last_field = fields[current_index-1];

            if (current_index < number_of_fields && field.address == last_field.address) {
                current_index++;
                continue;
            }

            if (current_index < number_of_fields && 
                field.address == fields[chunk_start_index].address + chunk_length_words && 
                chunk_length_words < max_run_length) {
                // Extend Run
                chunk_length_words += field.length();
                current_index++;
            } else {
                // Finish Run
                chunks.push_back(Chunk(fields[chunk_start_index].address, chunk_length_words, fields[chunk_start_index].buffer_position));
                debug_print("Run Length: "); debug_println(run_length_words);
                chunk_start_index = current_index;
                chunk_length_words = 0;
                if(current_index >= number_of_fields) {
                    at_the_end = true;
                }
            }
        }
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