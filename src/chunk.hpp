#pragma once 

#include <stdint.h>

struct Chunk {
    uint16_t start_address;
    uint16_t length;
    size_t buffer_position;

    Chunk(uint16_t start, uint16_t word_count, size_t buffer_position) {
        start_address = start;
        length = word_count;
        buffer_position = buffer_position;
    }
};