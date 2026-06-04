#include "eth_protocol.h"
#include <stdio.h>
#include <inttypes.h>

/**
 * @brief debugging function that prints an eth_protocol_test_t completely.
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
