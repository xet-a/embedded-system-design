#include "common.h"
#include "servo.h"
#include "game.h"
#include "bluetooth.h"
#include "light.h"
#include "motor.h"


void delay(int d){
  for (int i = 0; i <= d; i++) {
      ;
  }
} 

uint16_t light_num[4] = {
    GPIO_Pin_8,
    GPIO_Pin_9,
    GPIO_Pin_10,
    GPIO_Pin_11
};
uint16_t motor_num = GPIO_Pin_2;

volatile uint32_t ADC_Value[6];
GameStatus gameStatus;
uint16_t scoreResult[8];



void getDecStr(uint16_t* str, uint16_t val){
  uint16_t i;
  uint16_t len = 0;
  
  uint16_t valCopy = val;
  
  while (valCopy > 0) {
    len++;
    valCopy /= 10;
  }
  
  for(i=1; i<=len; i++){
    str[len-i] = (uint8_t) ((val % 10UL) + '0');
    val/=10;
  }

  str[i-1] = '\0';
}

/* ========================= SERVO ========================= */


PWM pwm, pwm2, pwm3, pwm4, pwm5;

static void pwm_setting(){
    pwm.OCMode     = TIM_OCMode_PWM1;
    pwm.rcc_timer    = RCC_APB1Periph_TIM4;
    pwm.timer           = TIM4;
    pwm.rcc_gpio     = RCC_APB2Periph_GPIOB;
    pwm.gpio_port    = GPIOB;
    pwm.gpio_pin     = GPIO_Pin_8;
    pwm.channel      = 3;
    SERVO_Configure(&pwm);
    SERVO_Rotate(&pwm, 90);
    
    pwm2.OCMode    = TIM_OCMode_PWM1;
    pwm2.rcc_timer   = RCC_APB1Periph_TIM4;
    pwm2.timer          = TIM4;
    pwm2.rcc_gpio    = RCC_APB2Periph_GPIOB;
    pwm2.gpio_port   = GPIOB;
    pwm2.gpio_pin    = GPIO_Pin_9;
    pwm2.channel     = 4;
    SERVO_Configure(&pwm2);
    SERVO_Rotate(&pwm2, 150);
    
    pwm3.OCMode    = TIM_OCMode_PWM1;
    pwm3.rcc_timer   = RCC_APB1Periph_TIM4;
    pwm3.timer          = TIM4;
    pwm3.rcc_gpio    = RCC_APB2Periph_GPIOB;
    pwm3.gpio_port   = GPIOB;
    pwm3.gpio_pin    = GPIO_Pin_6;
    pwm3.channel     = 1;
    SERVO_Configure(&pwm3);
    
    // start servo
    pwm4.OCMode    = TIM_OCMode_PWM1;
    pwm4.rcc_timer   = RCC_APB1Periph_TIM4;
    pwm4.timer          = TIM4;
    pwm4.rcc_gpio    = RCC_APB2Periph_GPIOB;
    pwm4.gpio_port   = GPIOB;
    pwm4.gpio_pin    = GPIO_Pin_7;
    pwm4.channel     = 2;
    SERVO_Configure(&pwm4);
    
    pwm5.OCMode    = TIM_OCMode_PWM1;
    pwm5.rcc_timer   = RCC_APB1Periph_TIM2;
    pwm5.timer          = TIM2;
    pwm5.rcc_gpio    = RCC_APB2Periph_GPIOA;
    pwm5.gpio_port   = GPIOA;
    pwm5.gpio_pin    = GPIO_Pin_3;
    pwm5.channel     = 4;
    SERVO_Configure(&pwm5);
}



/* ========================= GAME ========================= */

void GAME_init(){
    gameStatus.start = 0;
    // led ???? ???
}

void GAME_start(){
    if (gameStatus.start != 1){
        gameStatus.start = 1;
        gameStatus.score = 0;
        
        SERVO_Rotate(&pwm4, 180);
        delay(1000);
        SERVO_Rotate(&pwm4, 0);
        
        // led ???? ???
    }
}

void GAME_end(){
    if (gameStatus.start != 0){
        USART_SendData(USART2, 'e');
        gameStatus.start = 0;
        
        // led ???? ???
    }
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
      
    word = USART_ReceiveData(USART2);
    if (word == 'a') {
      printf("a\n");
      SERVO_Rotate(&pwm, 210);
    }
    else if (word == 'b') {
      printf("b\n");
      SERVO_Rotate(&pwm, 0);
    }
    else if (word == 'c') {
      printf("c\n");
      SERVO_Rotate(&pwm2, 0);
    }
    else if (word == 'd') {
      printf("d\n");
      SERVO_Rotate(&pwm2, 210);
    }
    /*else if (word == 's') {
      printf("game score : %d\n", gameStatus.score);
      USART_SendData(USART2, gameStatus.score);
    }*/
    else if (word == 'k') {
      printf("start button\n");
      GAME_start();
    }
    
    USART_SendData(USART1, word);
    USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }
}

/* ========================= LED ========================= */


/* ========================= SENSOR ========================= */

void sensor_RCC_Configure() {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  // DMA port clock enable
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

void sensor_GPIO_Configure() {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure_C;
  
  // ADC Port Configure
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure_C.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure_C.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure_C);
}

void sensor_ADC_Configure() {
  ADC_InitTypeDef ADC_InitStructure;
  
  // ADC Configure
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 6;
  
  ADC_Init(ADC1, &ADC_InitStructure);
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5); // PB0
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_239Cycles5); // PB1
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 3, ADC_SampleTime_239Cycles5); // PC0
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 4, ADC_SampleTime_239Cycles5); // PC1
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 5, ADC_SampleTime_239Cycles5); // PC2
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 6, ADC_SampleTime_239Cycles5); // PC3
  
  // Enable interrupt
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
  ADC_DMACmd(ADC1, ENABLE);
  
  // Enable ADC1
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);
  
  printf("1\n");
  
  while(ADC_GetResetCalibrationStatus(ADC1));
  
  printf("2\n");
  
  ADC_StartCalibration(ADC1);
  
  while(ADC_GetCalibrationStatus(ADC1));
  
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*
void sensor_NVIC_Configure() {
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  // NVIC Line ADC1
  NVIC_EnableIRQ(ADC1_2_IRQn);
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
*/

void sensor_DMA_Configure() {
  DMA_InitTypeDef DMA_Instructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  DMA_DeInit(DMA1_Channel1);
  /* DMA Configuration */
  ////////////////////////////////////////////////////////
  
  DMA_Instructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_Instructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Value;
  DMA_Instructure.DMA_DIR = DMA_DIR_PeripheralSRC; // Peripheral에서 데이터 가져옴
  
  DMA_Instructure.DMA_BufferSize = 6;
  
  DMA_Instructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_Instructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_Instructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_Instructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  
  DMA_Instructure.DMA_Mode = DMA_Mode_Circular; // Circular Mode
  DMA_Instructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_Instructure.DMA_M2M = DMA_M2M_Disable;
  
  /////////////////////////////////////////////////////////
  
  
  DMA_Init(DMA1_Channel1, &DMA_Instructure);
  DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  ADC_DMACmd(ADC1, ENABLE);
}

void DMA1_Channel1_IRQHandler() {
  if (DMA_GetITStatus(DMA1_IT_TC1) != RESET) {
    if (ADC_Value[0] < 500 || ADC_Value[1] < 500 || ADC_Value[2] < 500 
        || ADC_Value[3] < 1500 || ADC_Value[4] < 500) {
      
      printf("adc1: %d, adc2: %d, adc3: %d\nadc4: %d, adc5: %d, adc6: %d\n", ADC_Value[0], ADC_Value[1], ADC_Value[2], ADC_Value[3], ADC_Value[4], ADC_Value[5]);
      gameStatus.score++;
      printf("game score : %d\n", gameStatus.score);
      
      if(gameStatus.score == 5){
        SERVO_Rotate(&pwm3, 0);
        //SERVO_Rotate(&pwm5, 0);
        delay(500);
        SERVO_Rotate(&pwm3, 30);
        //SERVO_Rotate(&pwm5, 30);
        delay(500);
        SERVO_Rotate(&pwm3, 0);
        //SERVO_Rotate(&pwm5, 0);
      }
      
      getDecStr(scoreResult, gameStatus.score);
      USART_SendData(USART2, *scoreResult);
      
    }
    DMA_ClearITPendingBit(DMA1_IT_TC1);
  }
}

/*
void ADC1_2_IRQHandler() {
  if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET) {
    uint32_t adc_val = ADC_GetConversionValue(ADC1);
    if (adc_val < 500) {
      printf("Interrupt invoked2, ADC: %d\n", adc_val);
      
    }
    
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
  }
}
*/

 /* ======================================================== */


    
void GPIO_Configure(void){
    GPIO_InitTypeDef GPIO_InitStructure;    
    // ???
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;// INPUT PULL-DOWN
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

int main(){
    printf("Init\n");
    SystemInit();
    pwm_setting();
    BT_init(&BT);
    GAME_init();
    light_Init();
    sensor_RCC_Configure();
    sensor_GPIO_Configure();
    sensor_ADC_Configure();
    //sensor_NVIC_Configure();
    sensor_DMA_Configure();
    
    while(1) {
        /*if (score > 150) {
            GPIO_SetBits(GPIOE, GPIO_Pin_2);
        }
        GPIO_SetBits(GPIOE, GPIO_Pin_2);
        //printf("adc1: %d, adc2: %d\n", ADC_Value[0], ADC_Value[1]);
        delay(5000000);
        GPIO_ResetBits(GPIOE, GPIO_Pin_2);*/
      
    
    }
    return 0;
}
