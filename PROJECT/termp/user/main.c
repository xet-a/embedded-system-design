#include "common.h"
#include "servo.h"
#include "game.h"
#include "bluetooth.h"
#include "sensor.h"
#include "light.h"
#include "motor.h"


void delay(int d){
  for (int i = 0; i <= d; i++) {
      ;
  }
} 


uint32_t score = 0;
uint16_t light_num[4] = {
    GPIO_Pin_8,
    GPIO_Pin_9,
    GPIO_Pin_10,
    GPIO_Pin_11
};
uint16_t motor_num = GPIO_Pin_2;

volatile uint32_t ADC_Value[1];

/* ========================= SERVO ========================= 


PWM pwm;
PWM pwm2;

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

/* ========================== BT ==========================*/

BTConfig BT;

// USART Interrupt Handler

void USART1_IRQHandler() {
    uint16_t word;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
    // the most recent received data by the USART1 peripheral
    word = USART_ReceiveData(USART1);

    
    // ??->?????? ?????? (????)
    USART_SendData(USART2, word);

    // clear 'Read data register not empty' flag
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}

void USART2_IRQHandler(){
    uint16_t word;
    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){
    // the most recent received data by the USART2 peripheral

    // ?????? ???? ?????? ???? o??
    word = USART_ReceiveData(USART2);
    /*if (word == 'L') {
      printf("Left\n");
      PWM_Rotate(&pwm, 80);
      delay(800);
      PWM_Rotate(&pwm, 0);
    }
    else if (word == 'R') {
      printf("Right\n");
      PWM_Rotate(&pwm, 0);
      delay(800);
      PWM_Rotate(&pwm, 80);
    }*/
    USART_SendData(USART1, word);

    // clear 'Read data register not empty' flag
    USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }
}
/* ========================= GAME ========================= */

GameStatus gameStatus;

void GAME_init(){
    gameStatus.start = 0;
    // led ???? ???
}

void GAME_start(){
    if (gameStatus.start != 1){
        gameStatus.start = 1;
        gameStatus.score = 0;
        
        // TODO: ???? ???? ???? ??? (servo.c ????)
        //SERVO_Rotate(&pwm, 180);
        
        // led ???? ???
    }
}

void GAME_end(){
    if (gameStatus.start != 0){
        USART_SendData(USART2, 'e');
        gameStatus.start = 0;
        
        // TODO: ???? ???? ???? ??? (servo.c ????)
        //SERVO_Rotate(&pwm, 180);
        
        // led ???? ???
    }
}

/* ======================================================= */


/* ========================= SENSOR ========================= */

/*void ADC1_2_IRQHandler() {
  if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET) {
    uint32_t adc_val = ADC_GetConversionValue(ADC1);
    if (adc_val < 500) {
      printf("Interrupt invoked, ADC: %d\n", adc_val);
      score += 10;
      //USART_SendData(USART2, score);
      GPIO_SetBits(GPIOD, light_num[0]);
      GPIO_SetBits(GPIOD, light_num[1]);
      GPIO_SetBits(GPIOD, light_num[2]);
      GPIO_SetBits(GPIOD, light_num[3]);
      
      delay(100000);
      
      GPIO_ResetBits(GPIOD, light_num[0]);
      GPIO_ResetBits(GPIOD, light_num[1]);
      GPIO_ResetBits(GPIOD, light_num[2]);
      GPIO_ResetBits(GPIOD, light_num[3]);
    }
    
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
  }
}*/


 /* ======================================================== */


    
void GPIO_Configure(void){
    GPIO_InitTypeDef GPIO_InitStructure;    
    // ???
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;// INPUT PULL-DOWN
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}


void sensor_DMA_Configure(void) {
  DMA_InitTypeDef DMA_Instructure;
    
    DMA_DeInit(DMA1_Channel1);
    /* DMA Configuration */
    ////////////////////////////////////////////////////////
    
    DMA_Instructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_Instructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Value[0];
    DMA_Instructure.DMA_DIR = DMA_DIR_PeripheralSRC; // Peripheral에서 데이터 가져옴
    
    DMA_Instructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_Instructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_Instructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_Instructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    

    
    DMA_Instructure.DMA_Mode = DMA_Mode_Circular; // Circular Mode
    DMA_Instructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_Instructure.DMA_M2M = DMA_M2M_Disable;
    
    /////////////////////////////////////////////////////////
    
    
    DMA_Init(DMA1_Channel1, &DMA_Instructure);
    
    DMA_Cmd(DMA1_Channel1, ENABLE);
  /*DMA_InitTypeDef DMA_Instructure;
  
  DMA_DeInit(DMA1_Channel1);
  //* DMA Configuration 
  ////////////////////////////////////////////////////////
  
  DMA_Instructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_Instructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Value[0];
  DMA_Instructure.DMA_DIR = DMA_DIR_PeripheralSRC; // Peripheral에서 데이터 가져옴
  
  DMA_Instructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_Instructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_Instructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_Instructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_Instructure.DMA_BufferSize = 32;
  
  
  DMA_Instructure.DMA_Mode = DMA_Mode_Circular; // Circular Mode
  DMA_Instructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_Instructure.DMA_M2M = DMA_M2M_Disable;
  
  /////////////////////////////////////////////////////////
  
  
  DMA_Init(DMA1_Channel1, &DMA_Instructure);
  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  NVIC_InitTypeDef NVIC_InitStructure;  
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);*/
}

/*void DMA1_Channel1_IRQHandler(void) {
  if(DMA_GetITStatus(DMA1_IT_TC1)){
    DMA_ClearITPendingBit(DMA1_IT_TC1 | DMA1_IT_GL1);
  }
}*/

int main(){
    printf("Init\n");
    SystemInit();
    BT_init(&BT);
    //sensor_Init();
    sensor_DMA_Configure();
    light_Init();
    GAME_init();
    
    while(1) {
        /*if (score > 150) {
            GPIO_SetBits(GPIOE, motor_num);
        }*/
        
        printf("adc1: %d, adc2: %d\n", ADC_Value[0], ADC_Value[1]);
        delay(50000);
        /*PWM_Rotate(&pwm, 80);
        PWM_Rotate(&pwm2, 80);
        delay(800);
        PWM_Rotate(&pwm, 0);
        PWM_Rotate(&pwm2, 0);
        delay(800);*/
    }
    

    return 0;
}
