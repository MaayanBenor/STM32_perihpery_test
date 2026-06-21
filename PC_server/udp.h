#pragma once
#include "eth_protocol.h"

/**
 * @file udp.h
 * @brief UDP communication interface for the PC server.
 */

/**
 * @brief Create and bind the UDP socket.
 * @return int 0 on success, 1 on error.
 */
int udp_init();

/**
 * @brief Receive one UDP datagram containing a string.
 * @return int 0 on success, 1 on error.
 */
int udp_receive_message();

/**
 * @brief Receive one UDP datagram containing a eth_protocol_packet.
 * @return eth_protocol_result_t that was received.
 */
eth_protocol_result_t udp_receive_eth_result();


/**
 * @brief Send a UDP datagram.
 * @param message Null-terminated message to send.
 * @return int 0 on success, 1 on error.
 */
int udp_send_message(const char* message);

/**
 * @brief Send a UDP datagram.
 * @param packet a packed struct of data.
 * @return int 0 on success, 1 on error.
 */
int udp_send_eth_packet(const eth_protocol_test_t* packet);

/**
 * @brief Close UDP resources.
 * @return int 0 on success, 1 on error.
 */
int udp_close();