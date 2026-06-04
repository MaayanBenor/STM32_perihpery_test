#include "server_eth_packet.h"
#include "eth_protocol.h"
#include <FreeRTOS.h>
#include <task.h>
#include <main.h>
#include <lwip/sockets.h>
#include <stdio.h>
#include <errno.h>
#include <rtg.h> //for UART_DEBUG

#define PORT 5000


void server_eth_packet_task(void *argument){
	int sock;
	struct sockaddr_in addr, client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	eth_protocol_test_t rx_packet = {0};


	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sock < 0){
		perror("socket failed");
		while(1){}
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("bind failed");
		while(1){}
	}

	while(1){ //main loop of the task
		//Is ready to receive all the packet including max payload.
		if(recvfrom(sock, &rx_packet, MAX_TEST_PACKET_SZ, 0,
						(struct sockaddr *) &client_addr, &client_addr_len) == -1){
			perror("recvfrom faild");
			while(1){}
		}
		print_eth_test(&rx_packet);

		if(sendto(sock, &rx_packet, sizeof(rx_packet), 0, (struct sockaddr *) &client_addr, client_addr_len)
				<
			sizeof(rx_packet)){
			perror("sendto failed");
			while(1){}
		}

		//else received 0 and that's an empty UDP packet that we should ignore
	}
}
