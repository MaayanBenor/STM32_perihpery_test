#include "udp.h"
#include "eth_protocol.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

/**
 * @file udp.c
 * @brief UDP communication implementation for the PC server.
 */

/** @brief Local UDP port used by the server. */
#define PORT 5000
/** @brief Maximum receive buffer size in bytes. */
#define BUFFER_SIZE 512
/** @brief Target STM device IPv4 address. */
#define STM_IP "10.0.1.101"

static int g_sock;
static struct sockaddr_in g_addr, g_client_addr;
static socklen_t g_client_addr_len = sizeof(g_client_addr);

static char g_buffer[BUFFER_SIZE];

/**
 * @brief Initialize UDP socket and endpoint addressing.
 * @return int 0 on success, 1 on failure.
 */
int udp_init(){


    g_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(g_sock < 0){
        perror("socket failed");
        return 1;
    }
    
    g_addr.sin_family = AF_INET;
    g_addr.sin_port = htons(PORT);
    g_addr.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(g_sock, (struct sockaddr *)&g_addr, sizeof(g_addr)) < 0){
		perror("bind failed");
		return 1;
	}
    
    g_client_addr.sin_family = AF_INET;
    g_client_addr.sin_port = htons(PORT);

    int pton_rc = inet_pton(AF_INET, STM_IP, &g_client_addr.sin_addr);
    if (pton_rc == -1) {
        perror("inet_pton failed");
        return 1;
    }
    if (pton_rc == 0) {
        fprintf(stderr, "inet_pton failed: invalid IPv4 address: %s\n", STM_IP);
        return 1;
    }

    return 0;
}

/**
 * @brief Receive a UDP datagram, print it, and echo it back.
 * @return int 0 on success, 1 on failure.
 */
int udp_receive_message(){
    int g_client_msg_len = recvfrom(g_sock, g_buffer, sizeof(g_buffer) -1, 0, 
                            (struct sockaddr *) &g_client_addr, &g_client_addr_len);
    if(g_client_msg_len > 0){
        g_buffer[g_client_msg_len] = '\0';
        printf("Received: %s\n", g_buffer); //for debugging, need to unpack the packet here.
        if(udp_send_message(g_buffer) != 0) {return 1;}
    }
    else if (g_client_msg_len < 0){
        perror("recvfrom failed");
        return 1;
    }
    //else g_client_msg_len == 0 and is an empty packet. OK

    return 0;
}

/**
 * @brief Receive a UDP datagram, print it, and echo it back.
 * @return int 0 on success, 1 on failure.
 */
int udp_receive_eth_packet(){
    eth_protocol_test_t rx_packet = {0}; //TODO: choose where information is processed. In main?
    //Is ready to receive all the packet including max payload.
  	if(recvfrom(g_sock, &rx_packet, MAX_TEST_PACKET_SZ, 0,
			(struct sockaddr *) &g_client_addr, &g_client_addr_len) == -1){
        perror("recvfrom faild");
		return -1;
	}

    return 0;
}

/**
 * @brief Send a UDP datagram to the configured client address.
 * @param message Null-terminated payload to transmit.
 * @return int 0 on success, non-zero on failure.
 */
int udp_send_message(const char* message){    
    if(sendto(g_sock, message, strlen(message), 0,
             (struct sockaddr *) &g_client_addr, g_client_addr_len) == -1)
             {
                perror("sendto failed");
                return 1;
             }
    return 0;
}

/**
 * @brief Send a UDP datagram to the configured client address.
 * @param packet a packed struct of data.
 * @return int 0 on success, 1 on error.
 */
int udp_send_eth_packet(const eth_protocol_test_t* packet){    
    if(packet == NULL){
        return 1;
    }

    if(sendto(g_sock, packet, sizeof(*packet), 0,
             (struct sockaddr *) &g_client_addr, g_client_addr_len) == -1)
             {
                perror("sendto failed");
                return 1;
             }
    return 0;
}

/**
 * @brief Close the socket that the udp.c uses
 * @return int 0 on success, 1 on failure.
 */
int udp_close(){
    if(g_sock < 0){
        return 0; //Aleardy closed
    }

    if(close(g_sock) != 0){
        perror("udp_close failed");
        return 1;
    }

    g_sock = -1; //Will fail if used after this. 
    return 0;
}