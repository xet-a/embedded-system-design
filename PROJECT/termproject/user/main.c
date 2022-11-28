#include "common.h"
#include "servo.h"

void RCC_Configure(void);
void GPIO_Configure(void);
void TIM2_Configure(void);


PWM pwm;
PWM pwm2;

void delay(int d){
  for (int i = 0; i <= d; i++) {
      ;
  }
} 
static void pwm_setting(){
    pwm.OCMode     = TIM_OCMode_PWM1;
    pwm.rcc_timer    = RCC_APB1Periph_TIM4;
    pwm.timer           = TIM4;
    pwm.rcc_gpio     = RCC_APB2Periph_GPIOB;
    pwm.gpio_port    = GPIOB;
    pwm.gpio_pin     = GPIO_Pin_8;
    pwm.channel      = 3;
    PWM_Configure(&pwm);
    
    pwm2.OCMode    = TIM_OCMode_PWM1;
    pwm2.rcc_timer   = RCC_APB1Periph_TIM4;
    pwm2.timer          = TIM4;
    pwm2.rcc_gpio    = RCC_APB2Periph_GPIOB;
    pwm2.gpio_port   = GPIOB;
    pwm2.gpio_pin    = GPIO_Pin_9;
    pwm2.channel     = 4;
    PWM_Configure(&pwm2);
}

    
void RCC_Configure(void){
    //  APB1_TIM2,4 clock enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    //  APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configure(void){
    GPIO_InitTypeDef GPIO_InitStructure;    

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;// INPUT PULL-DOWN
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}


int main(){
    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    pwm_setting();
    
    
    while (1) {
      // TODO: 버튼 누르면 모터로 치는 부분 -> 블루투스 연동 후 앱 버튼으로 수정해야 함
      if (!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11)){
        change_pwm_cycle(&pwm, 180);
        delay(100000);
        change_pwm_cycle(&pwm, 0);
        
      }
      if (!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12)){
        change_pwm_cycle(&pwm2, 180);
        delay(100000);
        change_pwm_cycle(&pwm2, 0);
      }  
    }

    return 0;
}

