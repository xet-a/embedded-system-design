#include "servo.h"

/*void SERVO_RCC_Configure() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    //  APB1 clock (TIM2,4) enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    //  APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void SERVO_GPIO_Configure() {
    /*GPIO_InitTypeDef GPIO_A;
    GPIO_InitTypeDef GPIO_B; 

    // PA0, PA6
    GPIO_A.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_6;
    GPIO_A.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_A.GPIO_Mode = GPIO_Mode_AF_PP; // Alternate Function
    GPIO_Init(GPIOA, &GPIO_A);

    // PB6
    GPIO_B.GPIO_Pin = GPIO_Pin_6;
    GPIO_B.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_B.GPIO_Mode = GPIO_Mode_AF_PP; // Alternate Function
    GPIO_Init(GPIOB, &GPIO_B);
}*/

void SERVO_Configure(PWM* pwm){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    //  APB1 clock (TIM2,4) enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    //  APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
    GPIO_InitTypeDef GPIO_InitStructure; 
    GPIO_InitStructure.GPIO_Pin = pwm->gpio_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // Alternate Function
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // TIMER
    TIM_TimeBaseStructure.TIM_Period= 20000-1;
    TIM_TimeBaseStructure.TIM_Prescaler= (uint16_t)(SystemCoreClock/1000000)-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseInit(pwm->timer, &TIM_TimeBaseStructure);

    // PWM init
    TIM_OCInitStructure.TIM_OCMode      = pwm -> OCMode;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 1500;
    
       printf("%d \n", pwm->channel);
    
    // TODO: 채널 분리 작동 안 됨
    switch(pwm->channel){
    case 3:
      TIM_OC3Init(pwm->timer, &TIM_OCInitStructure);
       printf("yes3 \n");
      TIM_OC3PreloadConfig(pwm->timer, TIM_OCPreload_Disable);
      break;
    case 4:
      TIM_OC4Init(pwm->timer, &TIM_OCInitStructure);
       printf("yes4 \n");
      TIM_OC4PreloadConfig(pwm->timer, TIM_OCPreload_Disable);
      break;
    default:
      break;
    }
    
    TIM_ARRPreloadConfig(pwm->timer, ENABLE);
    TIM_Cmd(pwm->timer,ENABLE);
    //TIM_ITConfig(pwm->timer, TIM_IT_Update, ENABLE);
}

void SERVO_Rotate(PWM* pwm, int degree){
    // 0 ~ 180 degree movement
    int pwm_pulse = ((2300 - 700) / 180) * (degree - 15) + 700;
    TIM_OCInitTypeDef tim_oc_init_struct;
    tim_oc_init_struct.TIM_OCMode      = pwm->OCMode;
    tim_oc_init_struct.TIM_OCPolarity  = TIM_OCPolarity_High;
    tim_oc_init_struct.TIM_OutputState = TIM_OutputState_Enable;
    tim_oc_init_struct.TIM_Pulse       = pwm_pulse;
    // 딜레이 꼭 넣어줄 것
    switch(pwm->channel){
    case 3:
      TIM_OC3Init(pwm->timer, &tim_oc_init_struct);
       printf("반복3 \n");
      break;
    case 4:
      TIM_OC4Init(pwm->timer, &tim_oc_init_struct);
       printf("반복4 \n");
      break;
    default:
      break;
    }
}