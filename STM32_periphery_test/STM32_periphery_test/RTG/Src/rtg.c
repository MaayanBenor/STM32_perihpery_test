#include "rtg.h"
#include "rtg_tasks.h"

#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>


void rtg_init(void){
	rtg_tasks_create();
}





/*


static void hello_world(void *arg);

void rtg_task_create(){
	xTaskCreate(hello_world, "hello world", 2048, NULL, 7, NULL);
}

static void hello_world(void *arg){
	for(;;){
		printf("Hello World!\r\n");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
*/
