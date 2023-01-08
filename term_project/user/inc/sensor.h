#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "common.h"

void sensor_RCC_Configure();
void sensor_GPIO_Configure();
void sensor_ADC_Configure();
void sensor_NVIC_Configure();
void sensor_Init();


#endif