#include "FreeRTOS.h"
#include "task.h"

#include "rtg_tasks.h"
#include "server.h"



void rtg_tasks_create(void){
	xTaskCreate(server_task, "SERVER", 512, NULL, 2, NULL);

}
