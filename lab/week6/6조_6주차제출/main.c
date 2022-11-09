#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

#include "misc.h"

/* function prototype */
void RCC_Configure(void);
void GPIO_Configure(void);
void EXTI_Configure(void);
void USART1_Init(void);
void NVIC_Configure(void);

void EXTI15_10_IRQHandler(void);

void Delay(void);

void sendDataUART1(uint16_t data);

//---------------------------------------------------------------------------------------------------
int mode = 0; //Fixed 0 : A, 1 : B
int print = 0; //Fixed
char puttyPrint[] = "TEAM06\r\n"; //Fixed 
int led_out = 0; //Fixed led 0~3 out


void RCC_Configure(void)
{
	// TODO: Enable the APB2 peripheral clock using the function 'RCC_APB2PeriphClockCmd'
	
	/* UART TX/RX port clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //Fixed 
        
	/* JoyStick Up/Down port clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //Fixed GPIOC >> PC5-UP, PC2-DOWN
        
	/* LED port clock enable */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); //Fixed GPIOD >> PD2,3,4,7 - L1,L2,L3,L4
	
	/* USART1 clock enable */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //Fixed USART
		
	/* Alternate Function IO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	// TODO: Initialize the GPIO pins using the structure 'GPIO_InitTypeDef' and the function 'GPIO_Init'
	
    /* JoyStick up, down pin setting */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_2;      //Fixed 5 : UP, 2 : DOWN
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;               // Fixed IPD : INPUT PULL-DOWN
    GPIO_Init(GPIOC, &GPIO_InitStructure);                      // Fixed > GPIOC SET
    
    /* button pin setting */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                  //Fixed 11: BUTTON S1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;               // Fixed IPD : INPUT PULL-DOWN
    GPIO_Init(GPIOD, &GPIO_InitStructure);                      // Fixed > GPIOD SET
    
    /* LED pin setting*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
    /* UART pin setting */
    //TX
    GPIO_InitTypeDef GPIO_InitStructure_TX;                     //Fixed 변수선언
    GPIO_InitStructure_TX.GPIO_Pin = GPIO_Pin_9;                   //Fixed 9 : TX
    GPIO_InitStructure_TX.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_InitStructure_TX.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure_TX);
    
    //RX
    GPIO_InitTypeDef GPIO_InitStructure_RX;                     //Fixed 변수선언
    GPIO_InitStructure_RX.GPIO_Pin = GPIO_Pin_10;                  //Fixed 10 : RX
    //GPIO_InitStructure_RX.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure_RX.GPIO_Mode = GPIO_Mode_IPU|GPIO_Mode_IPD; //Fixed 
    GPIO_Init(GPIOA, &GPIO_InitStructure_RX);
}

void EXTI_Configure(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;

    // TODO: Select the GPIO pin (Joystick, button) used as EXTI Line using function 'GPIO_EXTILineConfig'
    // TODO: Initialize the EXTI using the structure 'EXTI_InitTypeDef' and the function 'EXTI_Init'
	
    /* Joystick Down */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Joystick Up */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;                  //Fixed C5 : UP
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Button */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource11);
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;                  //Fixed D11 : BUTTON S1
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
    // NOTE: do not select the UART GPIO pin used as EXTI Line here
}

void USART1_Init(void)
{
	USART_InitTypeDef USART1_InitStructure;

	// Enable the USART1 peripheral
	USART_Cmd(USART1, ENABLE);
	
	// TODO: Initialize the USART using the structure 'USART_InitTypeDef' and the function 'USART_Init'
        //Week 5 TODO6~10 참고
	USART1_InitStructure.USART_BaudRate = 9600;                                           //Fixed
        USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;      //Fixed Week5 TODO_10
        USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                      //Fixed
        USART1_InitStructure.USART_Parity = USART_Parity_No;                                  //Fixed
        USART1_InitStructure.USART_StopBits = USART_StopBits_1;                               //Fixed
        USART1_InitStructure.USART_WordLength = USART_WordLength_8b;                          //Fixed 8b : 8Bit
	
        USART_Init(USART1, &USART1_InitStructure);                                             //Fixed stm32f19x_usart.h
	
        // TODO: Enable the USART1 RX interrupts using the function 'USART_ITConfig' and the argument value 'Receive Data register not empty interrupt'
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                                        //Fixed stm32f19x_usart.c
        
}

void NVIC_Configure(void) {

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure_UP;
    NVIC_InitTypeDef NVIC_InitStructure_DOWN;
    NVIC_InitTypeDef NVIC_InitStructure_BUTT;
    
    // TODO: fill the arg you want
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //Fixed pre-enption 2bit, sub priority 2bit

    // TODO: Initialize the NVIC using the structure 'NVIC_InitTypeDef' and the function 'NVIC_Init'
    
    //부팅시 업부터 해야 순서대로 동작함.
    //UP
    NVIC_InitStructure_UP.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure_UP.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure_UP.NVIC_IRQChannelPreemptionPriority = 0x01; // TODO
    NVIC_InitStructure_UP.NVIC_IRQChannelSubPriority = 0x00; // TODO
    NVIC_Init(&NVIC_InitStructure_UP);

    //DOWN
    NVIC_InitStructure_DOWN.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure_DOWN.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure_DOWN.NVIC_IRQChannelPreemptionPriority = 0x02; // TODO
    NVIC_InitStructure_DOWN.NVIC_IRQChannelSubPriority = 0x00; // TODO
    NVIC_Init(&NVIC_InitStructure_DOWN);

    //BUTTON
    NVIC_InitStructure_BUTT.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure_BUTT.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure_BUTT.NVIC_IRQChannelPreemptionPriority = 0x03;  // TODO
    NVIC_InitStructure_BUTT.NVIC_IRQChannelSubPriority = 0x00;  // TODO
    NVIC_Init(&NVIC_InitStructure_BUTT); 
    // UART1
    //NVIC_InitTypeDef  NVIC_InitStructure_UART1;
	
    // 'NVIC_EnableIRQ' is only required for USART setting
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00 ; // TODO
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; // TODO
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USART1_IRQHandler() {
    uint16_t word;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
    	// the most recent received data by the USART1 peripheral
        word = USART_ReceiveData(USART1);

        // TODO implement
        if (word == 'a')               //Up
        {
          mode = 0;
        }
        else if (word == 'b')          //Down
        {
          mode = 1;
        }

        // clear 'Read data register not empty' flag
    	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}

void EXTI15_10_IRQHandler(void) { //Fixed button press

	if (EXTI_GetITStatus(EXTI_Line11) != RESET) {
		if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == Bit_RESET) {
			// TODO implement
			print = 1;
		}
        EXTI_ClearITPendingBit(EXTI_Line11);
	}
}

// TODO: Create Joystick interrupt handler functions
/* --- TODO START - Add IRQHandler functions */
void EXTI9_5_IRQHandler(void) { //Fixed JoyStick UP

	if (EXTI_GetITStatus(EXTI_Line5) != RESET) {
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == Bit_RESET) {
			mode = 0; // Change mode A when joystick is UP
		}
        EXTI_ClearITPendingBit(EXTI_Line5);
	}
}

void EXTI2_IRQHandler(void) { //Fixed JoyStick DOWN

	if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) == Bit_RESET) {
			mode = 1; // Change mode B when joystick is DOWN
		}
        EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

/* --- TODO END --- */

void Delay(void) {
	int i;

	for (i = 0; i < 2000000; i++) {}
}

void sendDataUART1(uint16_t data) {
	/* Wait till TC is set */
	while ((USART1->SR & USART_SR_TC) == 0);
	USART_SendData(USART1, data);
}

int main(void)
{

    SystemInit();

    RCC_Configure();

    GPIO_Configure();

    EXTI_Configure();

    USART1_Init();

    NVIC_Configure();

   uint16_t led[4] = {
      GPIO_Pin_2,
      GPIO_Pin_3,
      GPIO_Pin_4,
      GPIO_Pin_7
    };

    while (1) {
       // TODO: implement 
      GPIO_SetBits(GPIOD, led[led_out]);
      Delay();
      GPIO_ResetBits(GPIOD, led[led_out]);
      if (mode == 0) {
        while (led_out < 0) {
          led_out += 4;
        }
        led_out = (led_out+1)%4;
      }
      else {
        while (led_out < 0) {
          led_out += 4;
        }
        led_out = (led_out-1 +4)%4;
      }
      if (print == 1) {
        char *tmp = &puttyPrint[0];
        while (*tmp != '\0') {
          sendDataUART1(*tmp);
          tmp++;
        }
        print = 0;
      }
    }
    return 0;
}
