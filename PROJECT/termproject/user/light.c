#include "common.h"

void LED_Set_GPIO(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  // LED Output Mode
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin=(GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // ÁÖÆÄ¼ö
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void LED_Init(void) {
  // APB2 Clk setting
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  LED_Set_GPIO();
}

void LED_OnOff(int index, int on) {

 	if (on) {
    GPIO_ResetBits(GPIOD, GPIO_Pin_2);
   	GPIO_ResetBits(GPIOD, GPIO_Pin_3);
   	GPIO_ResetBits(GPIOD, GPIO_Pin_4);
   	GPIO_ResetBits(GPIOD, GPIO_Pin_7);
 		if (index == 0) {
 			GPIO_SetBits(GPIOD, GPIO_Pin_2);
 		} else if (index == 1) {
 			GPIO_SetBits(GPIOD, GPIO_Pin_3);
 		} else if (index == 2) {
 			GPIO_SetBits(GPIOD, GPIO_Pin_4);
 		} else if (index == 3) {
 			GPIO_SetBits(GPIOD, GPIO_Pin_7);
 		} else {

 		}
 	} else {
    if (index == 0) {
 			GPIO_ResetBits(GPIOD, GPIO_Pin_2);
 		} else if (index == 1) {
 			GPIO_ResetBits(GPIOD, GPIO_Pin_3);
 		} else if (index == 2) {
 			GPIO_ResetBits(GPIOD, GPIO_Pin_4);
 		} else if (index == 3) {
 			GPIO_ResetBits(GPIOD, GPIO_Pin_7);
 		} else {

 		}
  }
}