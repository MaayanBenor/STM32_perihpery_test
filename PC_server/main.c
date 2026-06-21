#include "udp.h"
#include "records.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/time.h>
#include <time.h>

//TODO: calc time of test send and overall test time
// static void fill_timestamp(char *buffer, size_t buffer_size)
// {
//     time_t now = time(NULL);
//     struct tm *local_time = localtime(&now);

//     if (local_time == NULL || strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", local_time) == 0) {
//         snprintf(buffer, buffer_size, "unknown");
//     }
// }

// static double calculate_duration_sec(const struct timeval *start, const struct timeval *end)
// {
//     return (double)(end->tv_sec - start->tv_sec) +
//            (double)(end->tv_usec - start->tv_usec) / 1000000.0;
// }

/**
 * @file main.c
 * @brief Entry point for the PC UDP server application.
 */

/**
 * @brief Initialize UDP communication, send a startup message, and wait for data.
 *
 * @return int Returns 0 on normal termination.
 */
//TODO: ask Leah if the CLI needs to be a continues program.
int main(void){
    // printf("\033[2J\033[H"); // Cleans the terminal
    test_record_t record = {.timestamp = "00:00", .duration_sec = 0};
    eth_protocol_result_t rx_packet;
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

    if (udp_send_eth_packet(&eth_test) != 0){
        (void)udp_close(); return 1;
    }

    rx_packet = udp_receive_eth_result();
    if (rx_packet.result == 0){
        perror("udp_receive_eth_result failed");
        return 1;
    }
    
    record.result = rx_packet.result;
    record.test_id = rx_packet.test_ID;
    save_test_record(&record);
    print_all_test_records();

    if (udp_close() != 0){
        return 1;}
        
    return 0;
}