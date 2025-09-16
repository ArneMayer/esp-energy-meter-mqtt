#pragma once 

#include <stdint.h>

struct Chunk {
    uint16_t start_address;
    size_t length;
    uint16_t buffer_position;

    Chunk(uint16_t start, size_t word_count, uint16_t buffer_position) {
        start_address = start;
        length = word_count;
        buffer_position = buffer_position;
    }
};