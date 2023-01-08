#ifndef __SERVO_H__
#define __SERVO_H__

#include "common.h"

typedef struct _PWM{
    int            channel;
    uint16_t       OCMode;
    uint32_t       rcc_timer;
    TIM_TypeDef*   timer;
    uint32_t       rcc_gpio;
    GPIO_TypeDef*  gpio_port;
    uint16_t       gpio_pin;
}PWM;

void SERVO_Configure(PWM*);
void SERVO_Rotate(PWM*, int);

#endif