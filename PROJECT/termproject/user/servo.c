#include "servo.h"

void PWM_Configure(PWM* pwm){
    GPIO_InitTypeDef GPIO_InitStructure; 
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // RCC, GPIO init
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    //  APB1_TIM2,4 clock enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    //  APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = pwm->gpio_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // Alternate Function
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // TIMER
    TIM_TimeBaseStructure.TIM_Period              = 20000-1;
    TIM_TimeBaseStructure.TIM_Prescaler         = (uint16_t)(SystemCoreClock/1000000)-1;
    TIM_TimeBaseStructure.TIM_ClockDivision    = 0;
    TIM_TimeBaseStructure.TIM_CounterMode    = TIM_CounterMode_Down;
    TIM_TimeBaseInit(pwm->timer, &TIM_TimeBaseStructure);

    // PWM init
    TIM_OCInitStructure.TIM_OCMode      = pwm->OCMode;
    TIM_OCInitStructure.TIM_OCPolarity                          = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState           = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 1500;
    
    // TODO: 채널 분리 작동 안 됨
    switch(pwm->channel){
    case 3:
      TIM_OC3Init(pwm->timer, &TIM_OCInitStructure);
      TIM_OC3PreloadConfig(pwm->timer, TIM_OCPreload_Disable);
      break;
    case 4:
      TIM_OC4Init(pwm->timer, &TIM_OCInitStructure);
      TIM_OC4PreloadConfig(pwm->timer, TIM_OCPreload_Disable);
      break;
    default:
      break;
    }
    
    TIM_ARRPreloadConfig(pwm->timer, ENABLE);
    TIM_Cmd(pwm->timer,ENABLE);
}


void PWM_Rotate(PWM* pwm, int degree){
    // 0 ~ 180 degree movement
    int pwm_pulse = ((2300 - 700) / 180) * (degree - 15) + 700;
    TIM_OCInitTypeDef tim_oc_init_struct;
    tim_oc_init_struct.TIM_OCMode      = pwm->OCMode;
    tim_oc_init_struct.TIM_OCPolarity  = TIM_OCPolarity_High;
    tim_oc_init_struct.TIM_OutputState = TIM_OutputState_Enable;
    tim_oc_init_struct.TIM_Pulse       = pwm_pulse;
    // 딜레이 꼭 넣어줄 것
    TIM_OC3Init(pwm->timer, &tim_oc_init_struct);
}