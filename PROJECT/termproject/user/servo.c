#include "servo.h"

void SERVO_RCC_Configure() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
}

void SERVO_GPIO_Configure() {
    GPIO_InitTypeDef GPIO_A;
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
}

void SERVO_TIM_Configure() {
    // TIM Clock Enable - TIM2,3,4는 APB1을 CLK source로 사용함
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // PA0
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // PA6
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // PB6

    // Enable TIM Global Interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    // 왜 TIM2인지 잘 모르겠다
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // TIM Initialize
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;    
    // 원래 period는 10k가 되어야 1s가 되지만 서보모터 날개가 너무 빠르게 움직여서 주기를 더 길게 만들어줌
    // period나 prescaler는 모두 0부터 시작하기 때문에 설정한 값에 -1을 해준다
    TIM_TimeBaseStructure.TIM_Period=20000-1; //2000-1; // 100micro second //100kHz
      // 1/72Mhz * 7200 * 10000 = 1s 을 이용해 
      // sysclk(72Mhz)에서 1M을 나누어 prescale을 7200으로 만드는 부분
    TIM_TimeBaseStructure.TIM_Prescaler=(uint16_t)(SystemCoreClock/1000000)-1;//720-1;
    TIM_TimeBaseStructure.TIM_ClockDivision=0;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Down; //Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    // TIM Output Channel 설정 (Channel 1 사용)
    TIM_OCInitTypeDef OutputChannel;
    OutputChannel.TIM_OCMode = TIM_OCMode_PWM1;
    OutputChannel.TIM_OutputState = TIM_OutputState_Enable;
    OutputChannel.TIM_OutputNState = TIM_OutputNState_Enable;
    OutputChannel.TIM_Pulse = 1500-1;
    //1500 // us(마이크로초) - 2300: 90도, 1500: 0도, 700: -90도(duty cycle 계산식 참고)
    OutputChannel.TIM_OCPolarity = TIM_OCPolarity_Low;
    OutputChannel.TIM_OCNPolarity = TIM_OCNPolarity_High;
    OutputChannel.TIM_OCIdleState = TIM_OCIdleState_Set;
    OutputChannel.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    // Init 후 Preload.. Disable하고 이후에 ARRPreloadConfig로 Enable하는데..
    // 꼭 필요한지 모르겠다.... 
    TIM_OC1Init(TIM2, &OutputChannel);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
    TIM_OC1Init(TIM3, &OutputChannel);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
    TIM_OC1Init(TIM4, &OutputChannel);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);
    // PWM은 ARR에 의해 주파수가 결정됨
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_ARRPreloadConfig(TIM4, ENABLE);

    // TIM Enable
    TIM_Cmd(TIM2,ENABLE);
    TIM_Cmd(TIM3,ENABLE);
    TIM_Cmd(TIM4,ENABLE);
    // Timer Interrupt ENABLE
    TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1 ,ENABLE); // interrupt enable
    TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_CC1 ,ENABLE); // interrupt enable
    TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_CC1 ,ENABLE); // interrupt enable
}

void SERVO_Init(void) {
    SERVO_RCC_Configure();
    SERVO_GPIO_Configure();
    SERVO_TIM_Configure();
}

void SERVO_Rotate(int select, int degree) {
    /* CCR 사용.. PWM은 ARR에 의해 주파수가 결정되고,
    // CCR에 의해 duty cycle이 결정된다...
    int cycle = 2000, minCycle = 60, maxCycle = 240;
    int lTime = (double)(cycle - (float)(maxCycle - minCycle) * (float)(degree / 180.0) - minCycle);
    int hTime = cycle - lTime;
    *ccr = lTime;*/

    
    // 0 ~ 180 degree movement
    int pwm_pulse = ((2300 - 700) / 180) * (degree - 15) + 700;
    TIM_OCInitTypeDef tim_oc_init_struct;
    tim_oc_init_struct.TIM_OCMode      = TIM_OCMode_PWM1;
    tim_oc_init_struct.TIM_OCPolarity  = TIM_OCPolarity_High;
    tim_oc_init_struct.TIM_OutputState = TIM_OutputState_Enable;
    tim_oc_init_struct.TIM_Pulse       = pwm_pulse;
    
    switch(select){
    case 2:
      TIM_Cmd(TIM2,ENABLE);
      TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
      break;
    case 3:
      TIM_Cmd(TIM3,ENABLE);
      TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
      break;
    case 4:
      TIM_Cmd(TIM3,ENABLE);
      TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
      break;
    default:
      break;
    }
}