#include "udp.h"
#include "records.h"
#include "timer.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>

/**
 * @file main.c
 * @brief Start of the PC UDP server application.
 */

/**
 * @brief Initialize UDP communication, send a startup message, and wait for data.
 *
 * @return int Returns 0 on normal termination.
 */
//TODO: Check all return values
int main(void){
    // printf("\033[2J\033[H"); // Cleans the terminal
    if(records_db_init() != 0){
        fprintf(stderr, "%s:%d: records_db_init failed.\n", __FILE__, __LINE__);
        return 1;
    }

    struct timespec start, end;

    eth_protocol_result_t rx_packet;
    eth_protocol_test_t eth_test = {0};
    eth_test.test_ID = 3;
    eth_test.peripheral_to_be_tested = 1;
    eth_test.num_of_test_iterations = 100;
    eth_test.payload_size = 3;
    eth_test.payload[0] = 0x11;
    eth_test.payload[1] = 0x22;
    eth_test.payload[2] = 0x33;


    if (udp_init() != 0){
        records_db_close();
        return 1;}


    if(clock_gettime(CLOCK_REALTIME, &start) != 0){
        perror("clock_gettime failed");
        records_db_close();
        (void)udp_close(); 
        return 1;
    }
    if (udp_send_eth_packet(&eth_test) != 0){
        records_db_close();
        (void)udp_close(); 
        return 1;
    }

    //TODO: figure out how to handle failure in main from this function
    rx_packet = udp_receive_eth_result();

    if(clock_gettime(CLOCK_REALTIME, &end) != 0){
        perror("clock_gettime failed");
        records_db_close();
        (void)udp_close(); 
        return 1;
    }

    if (rx_packet.result == 0){
        fprintf(stderr, "%s:%d: sent_at == NULL\n", __FILE__, __LINE__);

        (void)udp_close(); 
        records_db_close();
        return 1;
    }

    save_test_record(rx_packet.test_ID, start.tv_sec , elapsed_seconds(start, end), rx_packet.result);
    print_all_test_records();
    

    records_db_close();
    if (udp_close() != 0){
        records_db_close();
        return 1;}
    
    return 0;
}