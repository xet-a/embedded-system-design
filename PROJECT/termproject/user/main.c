#include "common.h"
#include "servo.h"
#include "game.h"
#include "bluetooth.h"



void delay(int d){
  for (int i = 0; i <= d; i++) {
      ;
  }
} 


/* ========================= SERVO ========================= */


/* ========================== BT ==========================*/

BTConfig BT;

// USART Interrupt Handler

void USART1_IRQHandler() {
    uint16_t word;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
    // the most recent received data by the USART1 peripheral
    word = USART_ReceiveData(USART1);

    
    // 컴->폰으로 보내기 (추정)
    USART_SendData(USART2, word);

    // clear 'Read data register not empty' flag
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}

void USART2_IRQHandler(){
    uint16_t word;
    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){
    // the most recent received data by the USART2 peripheral

    // 폰에서 보낸 데이터 받아서 처리
    word = USART_ReceiveData(USART2);
    if (word == 'L') {
      printf("Left\n");
      delay(800);
    }
    else if (word == 'R') {
      printf("Right\n");
      delay(800);
    }
    USART_SendData(USART1, word);

    // clear 'Read data register not empty' flag
    USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }
}
/* ========================= GAME ========================= */

GameStatus gameStatus;

void GAME_init(){
    gameStatus.start = 0;
    // led 점멸 추가
}

void GAME_start(){
    if (gameStatus.start != 1){
        gameStatus.start = 1;
        gameStatus.score = 0;
        
        // TODO: 서보 모터 구분 필요 (servo.c 수정)
        //PWM_Rotate(&pwm, 180);
        
        // led 점멸 추가
    }
}

void GAME_end(){
    if (gameStatus.start != 0){
        USART_SendData(USART2, 'e');
        gameStatus.start = 0;
        
        // TODO: 서보 모터 구분 필요 (servo.c 수정)
        //PWM_Rotate(&pwm, 180);
        
        // led 점멸 추가
    }
}

/* ======================================================= */


    
void GPIO_Configure(void){
    GPIO_InitTypeDef GPIO_InitStructure;    
    // 버튼
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;// INPUT PULL-DOWN
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}


int main(){
    SystemInit();
    BT_init(&BT);
    GAME_init();
    printf("no ok");
    SERVO_Init();
    printf("ok");
    
    while(1) {
        printf("ok1");
        setServo1(90);
        printf("ok2");
        delay(10000);
        setServo1(180);
        delay(10000);
        setServo2(90);
        printf("ok3");
        delay(10000);
        setServo2(180);
    }
    

    return 0;
}

