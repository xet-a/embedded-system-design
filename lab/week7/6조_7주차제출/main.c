#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

#include "misc.h"

/* function prototype */
void RCC_Configure(void);
void USART1_Init(void);
void USART2_Init(void);

//---------------------------------------------------------------------------------------------------


void RCC_Configure(void) {  
        /* UART TX/RX port clock enable */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
        
	/* USART1 clock enable */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	
        /* USART2 clock enable */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
        
	/* Alternate Function IO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void USART1_Init(void){
	USART_InitTypeDef USART1_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
        
        /* UART pin setting */
        //TX
        GPIO_InitTypeDef GPIO_InitStructure_TX;
        GPIO_InitStructure_TX.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure_TX.GPIO_Speed = GPIO_Speed_50MHz;   
        GPIO_InitStructure_TX.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure_TX);
        
        //RX
        GPIO_InitTypeDef GPIO_InitStructure_RX;
        GPIO_InitStructure_RX.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure_RX.GPIO_Mode = GPIO_Mode_IPU|GPIO_Mode_IPD;
        GPIO_Init(GPIOA, &GPIO_InitStructure_RX);

	// Enable the USART1 peripheral
	USART_Cmd(USART1, ENABLE);
	
	// Initialize the USART using the structure 'USART_InitTypeDef' and the function 'USART_Init'
	USART1_InitStructure.USART_BaudRate = 9600;                                      
        USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
        USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
        USART1_InitStructure.USART_Parity = USART_Parity_No;                                  
        USART1_InitStructure.USART_StopBits = USART_StopBits_1;                               
        USART1_InitStructure.USART_WordLength = USART_WordLength_8b;                          
	
        USART_Init(USART1, &USART1_InitStructure); 
	
        // Enable the USART1 RX interrupts using the function 'USART_ITConfig' and the argument value 'Receive Data register not empty interrupt'
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                                        
        
        // 'NVIC_EnableIRQ' is only required for USART setting
        NVIC_EnableIRQ(USART1_IRQn);
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01 ;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
}


void USART2_Init(void){
	USART_InitTypeDef USART2_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
        
        /* UART pin setting */
        //TX
        GPIO_InitTypeDef GPIO_InitStructure_TX;
        GPIO_InitStructure_TX.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure_TX.GPIO_Speed = GPIO_Speed_50MHz;   
        GPIO_InitStructure_TX.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure_TX);
        
        //RX
        GPIO_InitTypeDef GPIO_InitStructure_RX;
        GPIO_InitStructure_RX.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure_RX.GPIO_Mode = GPIO_Mode_IPU|GPIO_Mode_IPD;
        GPIO_Init(GPIOA, &GPIO_InitStructure_RX);

	// Enable the USART2 peripheral
	USART_Cmd(USART2, ENABLE);
	
	// Initialize the USART using the structure 'USART_InitTypeDef' and the function 'USART_Init'
	USART2_InitStructure.USART_BaudRate = 9600;                                      
        USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
        USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
        USART2_InitStructure.USART_Parity = USART_Parity_No;                                  
        USART2_InitStructure.USART_StopBits = USART_StopBits_1;                               
        USART2_InitStructure.USART_WordLength = USART_WordLength_8b;                          
	
        USART_Init(USART2, &USART2_InitStructure); 
	
        // Enable the USART2 RX interrupts using the function 'USART_ITConfig' and the argument value 'Receive Data register not empty interrupt'
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                                        
        
        // 'NVIC_EnableIRQ' is only required for USART setting
        NVIC_EnableIRQ(USART2_IRQn);
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01 ;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
}

void USART1_IRQHandler() {
    uint16_t word;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
    	// the most recent received data by the USART1 peripheral
        word = USART_ReceiveData(USART1);
        
        USART_SendData(USART2, word);

        // clear 'Read data register not empty' flag
    	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}

void USART2_IRQHandler() {
    uint16_t word;
    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){
    	// the most recent received data by the USART2 peripheral
        word = USART_ReceiveData(USART2);
        
        USART_SendData(USART1, word);

        // clear 'Read data register not empty' flag
    	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }
}

int main(void){
    SystemInit();
    RCC_Configure();    
    USART1_Init();
    USART2_Init();
    
    while (1) {
      ;
    }

    return 0;
}
