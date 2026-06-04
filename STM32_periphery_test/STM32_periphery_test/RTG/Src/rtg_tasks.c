#include "server_eth_packet.h"
#include <FreeRTOS.h>
#include <task.h>

#include <rtg_tasks.h>



void rtg_tasks_create(void){
	xTaskCreate(server_eth_packet_task, "SERVER", 512, NULL, 2, NULL);

}
