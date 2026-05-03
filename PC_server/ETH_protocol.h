#pragma once

#include <inttypes.h>

typedef struct __attribute__((packed)) {
    //header - overall size: 7 bytes
    uint32_t test_ID;
    uint8_t peripheral_to_be_tested;
    uint8_t num_of_test_iterations;
    uint8_t payload_size;
    //payload
    uint8_t payload[255];
} ETH_protocol_test;

typedef struct __attribute__((packed)) {
    //overall size - 5 bytes
    uint32_t test_id;
    uint8_t result;
} ETH_protocol_result;
