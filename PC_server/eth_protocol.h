#pragma once

#include <inttypes.h>

#define PACKET_HEADER_SZ 7
#define MAX_TEST_PACKET_SZ 262

/**
 * @file eth_protocol.h
 * @brief Packed Ethernet test protocol structures.
 */

/**
 * @brief Packet sent to the target.
 */
typedef struct __attribute__((packed)) {
    //header - overall size: 7 bytes
    uint32_t test_ID;
    uint8_t peripheral_to_be_tested; //make a bitwize map
    uint8_t num_of_test_iterations;
    uint8_t payload_size;
    //payload
    uint8_t payload[255];
} eth_protocol_test_t;

/**
 * @brief Packet returned by the target.
 */
typedef struct __attribute__((packed)) {
    //overall size - 5 bytes
    uint32_t test_id;
    uint8_t result;
} eth_protocol_result_t;

void print_eth_test(const eth_protocol_test_t *t);