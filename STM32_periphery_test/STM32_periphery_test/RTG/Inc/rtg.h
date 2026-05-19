#pragma once

#include "stm32f7xx_hal.h"
#include "usart.h"
#include <stdio.h>

#define UART_DEBUG &huart3

void rtg_init(void);



//void rtg_task_create();
