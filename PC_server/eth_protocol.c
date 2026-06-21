#include "eth_protocol.h"
#include <stdio.h>
#include <inttypes.h>

/**
 * @brief debugging func that prints an eth_protocol_test_t.
 *
 * @param t the struct holding the eth communication protocol.
 */

void print_eth_test(const eth_protocol_test_t *t){
     printf("test_ID=%" PRIu32 " peripheral=%u iterations=%u payload_size=%u payload=",
           t->test_ID,
           t->peripheral_to_be_tested,
           t->num_of_test_iterations,
           t->payload_size);

    for (uint8_t i = 0; i < t->payload_size; ++i) {
        printf(" %02X", t->payload[i]);
    }
    printf("\n");
}

/**
 * @brief debugging func that prints an eth_protocol_result_t.
 *
 * @param r the struct holding the eth communication protocol
 */

void print_eth_result(const eth_protocol_result_t *r){
    printf("test_ID=%" PRIu32 " test result=%u\n", r->test_ID, r->result);
}