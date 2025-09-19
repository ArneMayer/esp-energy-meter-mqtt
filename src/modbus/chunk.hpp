#pragma once 

#include <stdint.h>
#include <cstddef>

struct Chunk {
    uint16_t start_address;
    uint16_t length;
    size_t buffer_position;
    RegisterType register_type;

    Chunk(uint16_t start_address_, uint16_t length_, size_t buffer_position_, RegisterType register_type_)
        : start_address(start_address_), length(length_), buffer_position(buffer_position_), register_type{register_type_} {}
};