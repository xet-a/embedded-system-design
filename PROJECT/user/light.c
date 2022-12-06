#include "light.h"


void light_RCC_Configure() {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
}

void light_GPIO_Configure() {
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void light_Init() {
  light_RCC_Configure();
  light_GPIO_Configure();
}