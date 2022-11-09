#include "stm32f10x.h"
#include "core_cm3.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "lcd.h"
#include "touch.h"


void RCC_Configure(void);
void GPIO_Configure(void);
void PWM_Configure(void);
void TIM2_Configure(void);

uint16_t pos_x=0;
uint16_t pos_y=0;
uint16_t tim2_counter = 0;

int color[12] = { WHITE, CYAN, BLUE, RED, MAGENTA, LGRAY, GREEN, YELLOW, BROWN, BRRED, GRAY };
int led1 = 0, led2 = 0;
int btn_state = 0;

void delay(){
  for (int i = 0; i <= 10000000; i++) {
      ;
  }
}


// duty cycle 적용해서 모터에 가해지는 평균 전압을 제어함
void PWM_Configure(void){      
      TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
      TIM_OCInitTypeDef TIM_OCInitStructure;  
      
      // 1/72Mhz * 7200 * 10000 = 1s 을 이용해 
      // sysclk(72Mhz)에서 1M을 나누어 prescale을 7200으로 만드는 부분
      uint16_t prescale = (uint16_t) (SystemCoreClock / 1000000); // 7200

      // 원래 period는 10k가 되어야 1s가 되지만 서보모터 날개가 너무 빠르게 움직여서 주기를 더 길게 만들어줌
      // period나 prescaler는 모두 0부터 시작하기 때문에 설정한 값에 -1을 해준다
      TIM_TimeBaseStructure.TIM_Period = 20000-1;
      TIM_TimeBaseStructure.TIM_Prescaler = prescale-1;
      TIM_TimeBaseStructure.TIM_ClockDivision = 0;
      TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
      TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

      // 타이머 Output Channel 설정
      TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
      TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
      TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
      TIM_OCInitStructure.TIM_Pulse = 1500; // us(마이크로초) - 2300: 90도, 1500: 0도, 700: -90도(duty cycle 계산식 참고)
      TIM_OC3Init(TIM4, &TIM_OCInitStructure);
      TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Disable);
      
      // PWM은 ARR에 의해 주파수가 결정됨
      TIM_ARRPreloadConfig(TIM4, ENABLE);
      TIM_Cmd (TIM4,ENABLE);
}

// 서보모터 각도 조절
void rotate(int pulse) {
    TIM_OCInitTypeDef TIM_OCInitStructure;
  
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = pulse;
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
}


void TIM2_Configure(void) {
   NVIC_InitTypeDef NVIC_InitStructure;
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
   // Enable TIM2 Global Interrupt 
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

   NVIC_Init(&NVIC_InitStructure);
   
   // TIM2 Initialize  - 계산 방식은 PWM과 같음, 카운터 모드는 up임
   TIM_TimeBaseStructure.TIM_Period=10000-1;
   TIM_TimeBaseStructure.TIM_Prescaler=7200-1;
   TIM_TimeBaseStructure.TIM_ClockDivision=0;
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
   TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);   
   
   // TIM2 Enale
   TIM_Cmd(TIM2, ENABLE);
   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void TIM2_IRQHandler(void) {
  // update 인터럽트 플래그가 set 상태인지 확인하고
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
    // 인터럽트 플래그를 clear 해줍니다
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    
    if (btn_state) {
      tim2_counter++;
      printf("%d\n", tim2_counter);
      
      // (궁금했던 점)
      // led를 on/off 하는 데 각각1초씩 걸려서 다시 켜지는 데는 2초가 걸림
      // period나 prescaler 값 중 하나를 2로 나눌 경우 켜지는 데는 1초가 걸림
      // => 미션지에는 1초마다 토글이므로 다시 켜지는 데 2초 걸리는 게 맞다
      if (led1) {
        printf("led1 on\n");
        GPIO_SetBits(GPIOD, GPIO_Pin_2);
      } else {
        printf("led1 off\n");
        GPIO_ResetBits(GPIOD, GPIO_Pin_2);
      }
      led1 = !led1;
          
      if (tim2_counter % 5 == 0) {
        if (led2) {
        printf("led2 on\n---\n");
        GPIO_SetBits(GPIOD, GPIO_Pin_3);
      } else {
        printf("led2 off\n---\n");
          GPIO_ResetBits(GPIOD, GPIO_Pin_3);
        }
        led2 = !led2;
      }
    }
    
    // OFF 상태) LED 리셋
    else{
      tim2_counter = 0;
      GPIO_ResetBits(GPIOD, GPIO_Pin_2);
      GPIO_ResetBits(GPIOD, GPIO_Pin_3);
    }
  }
}

  
void RCC_Configure(void){
    //  APB1_TIM2,4 clock enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    //  APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}


void GPIO_Configure(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // PWM 서보 모터
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 서보모터는 Alternate Function(datasheet 참고)
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // LED1 (D2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    // LED2 (D3)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
      
}    



int main(){
    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    PWM_Configure();
    TIM2_Configure();

    LCD_Init();
    Touch_Configuration();
    //Touch_Adjust();
    LCD_Clear(WHITE);
 
    while (1) {
      /* 서보모터 90, -90 확인 시 아래 코드 주석 해제, 딜레이 필수(딜레이 낮으면 끝까지 안 돌아감), 딜레이 동작할 경우 터치 lcd 바로바로 인식 안 됨 주의 */
      /*
      delay();
      rotate(700);
      delay();
      rotate(2300);
      */
      
      LCD_ShowString(20, 20, "TUE_TEAM 06", color[2], color[0]);
      LCD_DrawRectangle(40, 60, 90, 110);
      LCD_ShowString(60, 80, "BUT", color[3], color[0]);

      if (btn_state) {
        LCD_ShowString(30, 40, "ON", color[3], color[0]);
      } else {
        LCD_ShowString(30, 40, "OFF", color[3], color[0]);
      }

      u16 new_pos_x = pos_x, new_pos_y = pos_y;
      Touch_GetXY(&new_pos_x, &new_pos_y, 0); // 터치
      
      /*  u16  con_x, con_y;
      delay();
      Convert_Pos(new_pos_x, new_pos_y, &con_x, &con_y); // 좌표변환
      printf("nx: %u, ny: %u\n", new_pos_x, new_pos_y);
      printf("cx: %u, cy: %u\n\n", con_x, con_y);*/

      if (new_pos_x != pos_x || new_pos_y != pos_y) {
        pos_x = new_pos_x;
        pos_y = new_pos_y;
        // 원래 코드로 동작하면 화면 아무데나 터치해도 인식이 돼서(그릴 때 적는 값이랑 터치 인식해서 받는 값이랑 차이가 나네요)
        // convert_pos로 변환해서 써야 하는데 함수가 제대로 동작을 안 해서(위 주석 부분)
        // 터치 받은 변수 값 대략 찍어보고 박스만 터치되게 했습니다
        if (pos_x >= 400 && pos_x <= 800 && pos_y >= 1300 && pos_y <= 1600 ) {
          btn_state = !btn_state;
          // ON 상태) Timer Interrupt ENABLE
          if (btn_state) {
            TIM_Cmd(TIM2,ENABLE);
            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
          }
          LCD_Clear(color[0]);   
        }
      }
      
    }
    
    return 0;
}
