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


// duty cycle �����ؼ� ���Ϳ� �������� ��� ������ ������
void PWM_Configure(void){      
      TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
      TIM_OCInitTypeDef TIM_OCInitStructure;  
      
      // 1/72Mhz * 7200 * 10000 = 1s �� �̿��� 
      // sysclk(72Mhz)���� 1M�� ������ prescale�� 7200���� ����� �κ�
      uint16_t prescale = (uint16_t) (SystemCoreClock / 1000000); // 7200

      // ���� period�� 10k�� �Ǿ�� 1s�� ������ �������� ������ �ʹ� ������ �������� �ֱ⸦ �� ��� �������
      // period�� prescaler�� ��� 0���� �����ϱ� ������ ������ ���� -1�� ���ش�
      TIM_TimeBaseStructure.TIM_Period = 20000-1;
      TIM_TimeBaseStructure.TIM_Prescaler = prescale-1;
      TIM_TimeBaseStructure.TIM_ClockDivision = 0;
      TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
      TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

      // Ÿ�̸� Output Channel ����
      TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
      TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
      TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
      TIM_OCInitStructure.TIM_Pulse = 1500; // us(����ũ����) - 2300: 90��, 1500: 0��, 700: -90��(duty cycle ���� ����)
      TIM_OC3Init(TIM4, &TIM_OCInitStructure);
      TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Disable);
      
      // PWM�� ARR�� ���� ���ļ��� ������
      TIM_ARRPreloadConfig(TIM4, ENABLE);
      TIM_Cmd (TIM4,ENABLE);
}

// �������� ���� ����
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
   
   // TIM2 Initialize  - ��� ����� PWM�� ����, ī���� ���� up��
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
  // update ���ͷ�Ʈ �÷��װ� set �������� Ȯ���ϰ�
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
    // ���ͷ�Ʈ �÷��׸� clear ���ݴϴ�
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    
    if (btn_state) {
      tim2_counter++;
      printf("%d\n", tim2_counter);
      
      // (�ñ��ߴ� ��)
      // led�� on/off �ϴ� �� ����1�ʾ� �ɷ��� �ٽ� ������ ���� 2�ʰ� �ɸ�
      // period�� prescaler �� �� �ϳ��� 2�� ���� ��� ������ ���� 1�ʰ� �ɸ�
      // => �̼������� 1�ʸ��� ����̹Ƿ� �ٽ� ������ �� 2�� �ɸ��� �� �´�
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
    
    // OFF ����) LED ����
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
    
    // PWM ���� ����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �������ʹ� Alternate Function(datasheet ����)
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
      /* �������� 90, -90 Ȯ�� �� �Ʒ� �ڵ� �ּ� ����, ������ �ʼ�(������ ������ ������ �� ���ư�), ������ ������ ��� ��ġ lcd �ٷιٷ� �ν� �� �� ���� */
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
      Touch_GetXY(&new_pos_x, &new_pos_y, 0); // ��ġ
      
      /*  u16  con_x, con_y;
      delay();
      Convert_Pos(new_pos_x, new_pos_y, &con_x, &con_y); // ��ǥ��ȯ
      printf("nx: %u, ny: %u\n", new_pos_x, new_pos_y);
      printf("cx: %u, cy: %u\n\n", con_x, con_y);*/

      if (new_pos_x != pos_x || new_pos_y != pos_y) {
        pos_x = new_pos_x;
        pos_y = new_pos_y;
        // ���� �ڵ�� �����ϸ� ȭ�� �ƹ����� ��ġ�ص� �ν��� �ż�(�׸� �� ���� ���̶� ��ġ �ν��ؼ� �޴� ���̶� ���̰� ���׿�)
        // convert_pos�� ��ȯ�ؼ� ��� �ϴµ� �Լ��� ����� ������ �� �ؼ�(�� �ּ� �κ�)
        // ��ġ ���� ���� �� �뷫 ���� �ڽ��� ��ġ�ǰ� �߽��ϴ�
        if (pos_x >= 400 && pos_x <= 800 && pos_y >= 1300 && pos_y <= 1600 ) {
          btn_state = !btn_state;
          // ON ����) Timer Interrupt ENABLE
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
