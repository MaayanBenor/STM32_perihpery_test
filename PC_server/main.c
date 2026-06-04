#include "udp.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

/**
 * @file main.c
 * @brief Entry point for the PC UDP server application.
 */

/**
 * @brief Initialize UDP communication, send a startup message, and wait for data.
 *
 * @return int Returns 0 on normal termination.
 */
int main(void){
    // printf("\033[2J\033[H"); // Cleans the terminal

    eth_protocol_test_t eth_test = {0};
    eth_test.test_ID = 1;
    eth_test.peripheral_to_be_tested = 1;
    eth_test.num_of_test_iterations = 100;
    eth_test.payload_size = 3;
    eth_test.payload[0] = 0x11;
    eth_test.payload[1] = 0x22;
    eth_test.payload[2] = 0x33;


    if (udp_init() != 0){
        return 1;}

    /*if (udp_send_message("Hello from computer") != 0){
        (void)udp_close(); return 1;}
    */

    if (udp_send_eth_packet(&eth_test) != 0){
        (void)udp_close(); return 1;
    }
    if (udp_receive_eth_packet() != 0){
        (void)udp_close(); return 1;}

    if (udp_close() != 0){
        return 1;}
        
    return 0;
}