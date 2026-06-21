#pragma once

#include <inttypes.h>

#define PACKET_HEADER_SZ 7
#define MAX_TEST_PACKET_SZ 262

// Bitmask values for peripheral_to_be_tested
#define PERIPH_TIMER 1u
#define PERIPH_UART  2u
#define PERIPH_SPI   4u
#define PERIPH_I2C   8u
#define PERIPH_ADC   16u

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
    uint8_t peripheral_to_be_tested; //bitmask to be used with OR: PERIPH_TIMER | PERIPH_UART | ...
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
    uint32_t test_ID;
    uint8_t result; //success = 1, failure = 255, network error = 0
} eth_protocol_result_t;
    
void print_eth_test(const eth_protocol_test_t *t);
void print_eth_result(const eth_protocol_result_t *r);