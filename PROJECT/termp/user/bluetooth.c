#include "common.h"
#include "bluetooth.h"


void bt_init(BTConfig *config);
int bt_send_data(const char *data, BTConfig *config);

void BT_RCC_Configure(void) {  
    /* UART TX/RX port clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
    /* USART1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
    /* USART2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
    /* Alternate Function IO clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void BT_GPIO_Configure(void) {
    /* UART1 pin setting */
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
    
    /* UART2 pin setting */
    //TX
    GPIO_InitStructure_TX.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure_TX.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_InitStructure_TX.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure_TX);  
    //RX
    GPIO_InitStructure_RX.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure_RX.GPIO_Mode = GPIO_Mode_IPU|GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure_RX);
}

void BT_UART_Configure(void) {
    USART_InitTypeDef USART1_InitStructure;
    USART_InitTypeDef USART2_InitStructure;

    USART1_InitStructure.USART_BaudRate = 9600;                                      
    USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
    USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
    USART1_InitStructure.USART_Parity = USART_Parity_No;                                  
    USART1_InitStructure.USART_StopBits = USART_StopBits_1;                               
    USART1_InitStructure.USART_WordLength = USART_WordLength_8b;                          
    USART_Init(USART1, &USART1_InitStructure);

    USART2_InitStructure.USART_BaudRate = 9600;                                      
    USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
    USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
    USART2_InitStructure.USART_Parity = USART_Parity_No;                                  
    USART2_InitStructure.USART_StopBits = USART_StopBits_1;                               
    USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART2, &USART2_InitStructure); 

    // Enable the USART peripheral
    USART_Cmd(USART1, ENABLE);
    USART_Cmd(USART2, ENABLE);

    // Enable the USART RX interrupts using the function 'USART_ITConfig' and the argument value 'Receive Data register not empty interrupt'
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);      
}

void BT_NVIC_Configure(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup2);

    // 'NVIC_EnableIRQ' is only required for USART setting
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void BT_init(BTConfig *config){
    config->isValid = false;
    config->isATscan = false;
    BT_RCC_Configure();
    BT_GPIO_Configure();
    BT_UART_Configure();
    BT_NVIC_Configure();  
}

int BT_send_data(const char *data, BTConfig *config){
    int i = 0;
    // 블루투스 정상 연결
    if(config->isValid == false) return 0;
    
    for(i=0; i<strlen(data); i++){
        USART_SendData(USART2, data[i]);
        delay_ms(10);
    }
    USART_SendData(USART2, '\r');
    delay_ms(10);
    config->isValid = false;
    return 1;
}