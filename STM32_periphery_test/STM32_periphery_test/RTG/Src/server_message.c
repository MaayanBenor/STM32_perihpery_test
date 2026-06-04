#include "server_message.h"
#include <eth_protocol.h>
#include <FreeRTOS.h>
#include <task.h>
#include <main.h>
#include <lwip/sockets.h>
#include <stdio.h>
#include <errno.h>
#include <rtg.h> //for UART_DEBUG

#define PORT 5000
#define BUFFER_SIZE 512


void server_message_task(void *argument){
	int sock, client_msg_len;
	struct sockaddr_in addr, client_addr;
	socklen_t client_addr_len = sizeof(client_addr);


	char buffer[BUFFER_SIZE];

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
		client_msg_len = recvfrom(sock, buffer, sizeof(buffer) -1, 0,
						(struct sockaddr *) &client_addr, &client_addr_len);
		if(client_msg_len > 0){
			buffer[client_msg_len] = '\0';
			printf("Received from computer: %s\r\n", buffer);

			if( sendto(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &client_addr, client_addr_len)
				<
				sizeof(buffer)){
				perror("sendto failed");
				while(1){}
			}
		}
		else if(client_msg_len < 0){
			perror("recvfrom failed");
			while(1){}
		}
		//else received 0 and that's an empty UDP packet that we should ignore
	}
}
