#pragma once

#include <stdint.h>
#include <vector>
#include <map>
#include <pins_arduino.h>

#include "debug.hpp"
#include "field.hpp"
#include "chunk.hpp"
#include "modbus_connection.hpp"

class ModbusDevice {

public:
    const uint16_t modbus_id;
    const uint16_t max_chunk_size = 20;
    const RegisterType register_type;

protected:
    std::vector<Field> _fields;
    std::shared_ptr<ModbusConnection> _con;
    
    std::map<uint16_t, size_t> _buffer_positions;
    std::vector<uint16_t> _buffer;
    std::vector<Chunk> _chunks;

    std::map<Field, unsigned long> _update_timestamps;
    std::map<Field, float> _field_values;
    

public:
    ModbusDevice(std::shared_ptr<ModbusConnection> con, uint16_t modbus_id, RegisterType reg_type, const std::vector<Field>& fields, uint16_t max_chunk_size) : 
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

        debug_println("Modbus ID: " + String(modbus_id));
        debug_println("Max Chunk Size: " + String(max_chunk_size));
        debug_println("Number of Fields: " + String(_fields.size()));
        debug_println("Number of Chunks: " + String(_chunks.size()));
        debug_println("Buffer Size: " + String(_buffer.size()));
    }

    
    void update_all() {
        for(const auto& chunk : _chunks) {
            uint16_t* data = &_buffer[chunk.buffer_position];
            _con->read_and_get(register_type, modbus_id, chunk.start_address, chunk.length, data);
        }   

        for (const auto& field : _fields) {
            if(field.enabled) {
                // Parse value
                uint16_t* data = &_buffer[_buffer_positions[field.address]];
                float value = parse_value(data, field);
                _field_values[field] = value;
                _update_timestamps[field] = millis();

                // Print debug output
                //debug_print(field.description); debug_print(": "); debug_print(value); debug_println(field.unit);
            }
        }
    }

    void update(const Field& field) {
        auto buffer_position = _buffer_positions.find(field.address);
        if (buffer_position != _buffer_positions.end()) {
            uint16_t* data = &_buffer[buffer_position->second];
            _con->read_and_get(register_type, modbus_id, field.address, field.length(), data);
            float value = parse_value(data, field);
            _field_values[field] = value;
            _update_timestamps[field] = millis();
        }
    }

    const std::map<Field, float>& values() const {
        return _field_values;
    }

    std::optional<float> value(const Field& field) const {
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

    std::optional<Field> field(const char* field_name) const {
        for(const auto& field : _fields) {
            if (strcmp(field_name, field.name) == 0) {
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

            if (!field.enabled) {
                continue;
            }

            // Extend Chunk
            uint16_t new_chunk_length = field.address + field.length() - chunk.start_address;
            if (field.address <= chunk.start_address + chunk.length && new_chunk_length <= max_chunk_size) {
                chunk.length = std::max(chunk.length, new_chunk_length);
            }
            // Start New Chunk
            else {
                chunks.push_back({field.address, field.length(), _buffer_positions[field.address]});
            }
        }

        #if DEBUG_PRINTS == true
        for (size_t i = 0; i < fields.size(); i++) {
            const Field& field = fields[i];
            debug_println("Field " + String(i) + ": buffer_pos " + String(_buffer_positions[field.address]));
        }
        for (size_t i = 0; i < chunks.size(); i++) {
            const Chunk& chunk = chunks[i];
            debug_println("Chunk " + String(i) + ": start: " + String(chunk.start_address) + ", length: " + String(chunk.length) + ", buffer_pos: " + String(chunk.buffer_position));
        }
        #endif

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

    float parse_value(uint16_t* data, const Field& field) {
        switch (field.type) {
            case FieldType::float32:
                return get_float(data) * field.factor;
            case FieldType::float32_reversed:
                return get_float_reversed(data) * field.factor;
            case FieldType::int16:
                return static_cast<float>(get_int16_t(data)) * field.factor;
            case FieldType::int32:
                return static_cast<float>(get_int32_t(data)) * field.factor;
            case FieldType::uint16:
                return static_cast<float>(get_uint16_t(data)) * field.factor;
            case FieldType::uint32:
                return static_cast<float>(get_uint32_t(data)) * field.factor;
            case FieldType::uint8_high_byte:
                return static_cast<float>(get_uint8_t_high_byte(data)) * field.factor;
            case FieldType::uint8_low_byte:
                return static_cast<float>(get_uint8_t_low_byte(data)) * field.factor;
            default:
                return std::numeric_limits<float>::quiet_NaN();
        }
    }
};