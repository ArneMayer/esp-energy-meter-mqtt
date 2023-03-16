#pragma once

#include <WString.h>
#include <stdint.h>

#include "logging.hpp"

class FieldsRun {
public:
    uint16_t start_address;
    uint16_t number_of_words;
    uint16_t data_map;

    FieldsRun(uint16_t start, uint16_t word_count, uint16_t storage_map) {
        start_address = start;
        number_of_words = word_count;
        data_map = storage_map;
    }
};

class EnergyMeterConfig {
public:
    uint8_t number_of_fields;
    RegisterType register_type;
    size_t data_per_meter;
    size_t max_run_length = 20;
    bool auto_config = true;
    
    String* field_name;
    uint16_t* field_address;
    String* field_description; 
    String* field_unit;
    float* field_factor;
    FieldType* field_data_type;
    uint16_t* field_data_map;
    bool* field_enabled;

    std::vector<FieldsRun> run_list;

    EnergyMeterConfig(uint16_t num_fields, RegisterType reg_type) {
        number_of_fields = num_fields;
        register_type = reg_type;

        field_name = new String[num_fields];
        field_address = new uint16_t[num_fields];
        field_description = new String[num_fields]; 
        field_unit = new String[num_fields];
        field_factor = new float[num_fields];
        field_data_type = new FieldType[num_fields];
        field_data_map = new uint16_t[num_fields];
        field_enabled = new bool[num_fields];
    }

    size_t setup_data_map() {
        size_t data_map = 0;
        for(int i = 0; i < number_of_fields; i++) {
            if (field_enabled[i]) {
                debug_print("field_data_map["); debug_print(i); debug_print("] = "); debug_println(data_map);
                field_data_map[i] = data_map;
                if (i < number_of_fields - 1 && (field_address[i+1] == field_address[i])) {
                    data_map += 0;
                } else {
                    data_map += field_length_from_type(field_data_type[i]);
                }
            }
        }

        return data_map;
    }

    void setup_run_list() {
        uint16_t current_index = 1;
        bool at_the_end = false;
        uint16_t run_start_index = 0;
        uint16_t run_length_words = field_length(0);

        while(!at_the_end) {
            if (current_index < number_of_fields) {
                uint16_t next_address = field_address[current_index];
                uint16_t last_address = field_address[current_index-1];
                if (next_address == last_address) {
                    current_index++;
                    continue;
                }
            }

            if (current_index < number_of_fields && 
                field_address[current_index] == field_address[run_start_index] + run_length_words && 
                run_length_words < max_run_length) {
                // Extend Run
                run_length_words += field_length(current_index); // TODO: uint8
                current_index += 1;
            } else {
                // Finish Run
                run_list.push_back(FieldsRun(field_address[run_start_index], run_length_words, field_data_map[run_start_index]));
                debug_print("Run Length: "); debug_println(run_length_words);
                run_start_index = current_index;
                run_length_words = 0;
                if(current_index >= number_of_fields) {
                    at_the_end = true;
                }
            }
        }
    }

    size_t field_length_from_type(FieldType type) {
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

    size_t field_length(size_t index) {
        return field_length_from_type(field_data_type[index]);
    } 

    void setup() {
        setup_registers();
        if (auto_config) {
            data_per_meter = setup_data_map();
            setup_run_list();
        }
    }

    virtual void setup_registers() = 0;
};