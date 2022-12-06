#ifndef __SERVO_H__
#define __SERVO_H__

#include "common.h"

typedef struct _PWM{
    int            channel;
    uint16_t       OCMode;
    uint32_t       rcc_timer;
    TIM_TypeDef*   timer;
}PWM;

void SERVO_Init(void);
void SERVO_Configure(PWM*);
void SERVO_Rotate(PWM*, int);

#endif