#pragma once 

#include <stdint.h>
#include <cstddef>

struct Chunk {
    uint16_t start_address;
    uint16_t length;
    size_t buffer_position;

    Chunk(uint16_t start_address_, uint16_t length_, size_t buffer_position_)
        : start_address(start_address_), length(length_), buffer_position(buffer_position_) {}
};