#include "sensor.h"

void sensor_RCC_Configure() {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}

void sensor_GPIO_Configure() {
  GPIO_InitTypeDef GPIO_InitStructure;
  
  // ADC Port Configure
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void sensor_ADC_Configure() {
  ADC_InitTypeDef ADC_InitStructure;
  
  // ADC Configure
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 2;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_239Cycles5);
  // ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 3, ADC_SampleTime_239Cycles5);
  
  // Enable interrupt
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
  
  // Enable ADC1
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);
  
  while(ADC_GetResetCalibrationStatus(ADC1));
  
  ADC_StartCalibration(ADC1);
  
  while(ADC_GetCalibrationStatus(ADC1));
  
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

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

void sensor_Init() {
  sensor_RCC_Configure();
  sensor_GPIO_Configure();
  sensor_ADC_Configure();
  sensor_NVIC_Configure();
}