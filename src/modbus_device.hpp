#pragma once

#include <WString.h>
#include <stdint.h>
#include <vector>
#include <map>
#include <pins_arduino.h>

#include "logging.hpp"
#include "field.hpp"
#include "chunk.hpp"
#include "modbus_connection.hpp"

class ModbusDevice {

public:
    const uint16_t modbus_id;
    const size_t max_chunk_size = 20;
    const RegisterType register_type;

protected:
    std::vector<Field> _fields;
    std::shared_ptr<ModbusConnection> _con;
    
    std::map<uint16_t, uint16_t> _buffer_positions;
    std::vector<uint16_t> _buffer;
    std::vector<Chunk> _chunks;

    std::map<Field, String> _field_values;
    

public:
    ModbusDevice(std::shared_ptr<ModbusConnection> con, uint16_t modbus_id, RegisterType reg_type, std::vector<Field> fields, size_t max_chunk_size) : 
        modbus_id{modbus_id},
        max_chunk_size{max_chunk_size}, 
        register_type{reg_type},
        _fields{fields},
        _con{con} {

        std::sort(_fields.begin(), _fields.end(), [](const Field& a, const Field& b){
            return a.address < b.address;
        }); 

        size_t buffer_size = setup_buffer_map();
        _buffer.resize(buffer_size);
        _chunks = setup_chunks(_fields);
    }

    
    void update_all() {
        for(const auto& chunk : _chunks) {
            uint16_t* data = &_buffer.at(chunk.buffer_position);
            _con->read_and_get(register_type, modbus_id, chunk.start_address, chunk.length, data);
        }   

        for (const auto& field : _fields) {
            if(field.enabled) {
                // Parse value
                uint16_t* data = &_buffer.at(_buffer_positions[field.address]);
                String value = get_value_as_string(data, field.type, field.factor);
                _field_values[field] = value;

                // Print debug output
                debug_print(field.description); debug_print(": "); debug_print(value); debug_println(field.unit);

            }
        }
    }

    void update(const Field& field) {
        auto buffer_position = _buffer_positions.find(field.address);
        if (buffer_position != _buffer_positions.end()) {
            uint16_t* data = &_buffer.at(buffer_position->second);
            _con->read_and_get(register_type, modbus_id, field.address, field.length(), data);
            String value = get_value_as_string(data, field.type, field.factor);
            _field_values[field] = value;
        }
    }

    const std::map<Field, String>& values() const {
        return _field_values;
    }

    std::optional<String> value(const Field& field) const {
        auto it = _field_values.find(field);
        if (it != _field_values.end()) {
            return _field_values.at(field);
        } else {
            return std::nullopt;
        }
    }

    const std::vector<Field>& fields() const {
        return _fields;
    }

    std::optional<Field> field(String field_name) {
        for(const auto& field : _fields) {
            if (field.name == field_name) {
                return field;
            }
        }

        return std::nullopt;
    }

private:
    size_t setup_buffer_map() {
        size_t buffer_size = 0;

        for(const Field& field : _fields) {
            if (field.enabled && _buffer_positions.find(field.address) == _buffer_positions.end()) {
                _buffer_positions[field.address] = buffer_size;
                buffer_size += field.length();
            }
        }

        return buffer_size;
    }

    std::vector<Chunk> setup_chunks(const std::vector<Field>& fields) {
        std::vector<Chunk> chunks;

        if (fields.empty()) {
            return chunks;
        }

        chunks.push_back({fields[0].address, fields[0].length(), _buffer_positions[fields[0].address]});

        for (size_t i = 1; i < fields.size(); i++) {
            const Field& field = fields[i];
            Chunk& chunk = chunks.back();

            // Extend Chunk
            size_t new_chunk_length = field.address + field.length() - chunk.start_address;
            if (field.address <= chunk.start_address + chunk.length && new_chunk_length < max_chunk_size) {
                chunk.length = new_chunk_length;
            }
            // Start New Chunk
            else {
                chunks.push_back({field.address, field.length(), _buffer_positions[field.address]});
            }
        }

        return chunks;
    }
    
    float get_float_reversed(uint16_t* data) {
        uint16_t swap[2];
        swap[0] = data[1];
        swap[1] = data[0];

        float f;
        std::memcpy(&f, swap, sizeof(float));
        return f;
    }

    float get_float(uint16_t* data) {
        float f;
        std::memcpy(&f, data, sizeof(float));
        return f;
    }

    constexpr uint32_t get_uint32_t(uint16_t* data) {
        return ((uint32_t)data[0] << 16) + (uint32_t)data[1];
    }

    constexpr int32_t get_int32_t(uint16_t* data) {
        return (int32_t)get_uint32_t(data);
    }

    constexpr uint16_t get_uint16_t(uint16_t* data) {
        return data[0];
    }

    constexpr int16_t get_int16_t(uint16_t* data) {
        return (int16_t)data[0];
    }

    constexpr uint8_t get_uint8_t_high_byte(uint16_t* data) {
        return (data[0] & (uint16_t)0xFF00) >> 8;
    }

    constexpr uint8_t get_uint8_t_low_byte(uint16_t* data) {
        return data[0] & (uint16_t)0x00FF;
    }

    String get_value_as_string(uint16_t* data, FieldType type, float field_factor) {
        if (type == FieldType::float32) {
            return String(get_float(data)*field_factor);
        } else if (type == FieldType::float32_reversed) {
            return String(get_float_reversed(data)*field_factor);
        } else if (type == FieldType::int16) {
            return String(get_int16_t(data)*field_factor);
        } else if (type == FieldType::int32) {
            return String(get_int32_t(data)*field_factor);
        } else if (type == FieldType::uint16) {
            return String(get_uint16_t(data)*field_factor);
        } else if (type == FieldType::uint32) {
            return String(get_uint32_t(data)*field_factor);
        } else if (type == FieldType::uint8_high_byte) {
            return String(get_uint8_t_high_byte(data)*field_factor);
        } else if (type == FieldType::uint8_low_byte) {
            return String(get_uint8_t_low_byte(data)*field_factor);
        } else {
            return "Unknown Type";
        }
    }
};