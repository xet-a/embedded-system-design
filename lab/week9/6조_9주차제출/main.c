#include "stm32f10x.h"
#include "core_cm3.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "lcd.h"
#include "touch.h"

#define LCD_NAME_POS_X 20
#define LCD_NAME_POS_Y 50
#define LCD_X_POS_X 68
#define LCD_X_POS_Y 70
#define LCD_Y_POS_X 68
#define LCD_Y_POS_Y 90
#define LCD_LUXVAL_X 5
#define LCD_LUXVAL_Y 110

void RCC_Configure(void);
void GPIO_Configure(void);
void ADC_Configure(void);
void ADC1_2_IRQHandler();
void NVIC_Configure(void);

uint16_t pos_x;
uint16_t pos_y;
uint16_t cvt_pos_x;
uint16_t cvt_pos_y;
uint16_t value;

int color[12] = { WHITE, CYAN, BLUE, RED, MAGENTA, LGRAY, GREEN, YELLOW, BROWN, BRRED, GRAY };


void delay()
{
  for (int i = 0; i <= 100000; i++) {
      ;
  }
}


void RCC_Configure(void)
{
    // DB port clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    
    // CS port clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    // WR, RS(D/C), RD port clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    
    // ADC port clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}


void GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // ADC Port Configure
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // Analog Input
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void ADC_Configure()
{
    ADC_InitTypeDef ADC_InitStructure;
 
    // ADC Configure
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // Independent Mode
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // Continuous conversion Mode
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 내부 타이머 이벤트 없음
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5); // sampling cycle 가장 높게

    // Enable interrupt
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

    // Enable ADC1
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);

    while(ADC_GetResetCalibrationStatus(ADC1));
 
    ADC_StartCalibration(ADC1); // ADC Calibration start

    while(ADC_GetCalibrationStatus(ADC1));

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


void NVIC_Configure(void)
{
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

void ADC1_2_IRQHandler()
{
    if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET) {
        value = ADC_GetConversionValue(ADC1);
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    }
}

int main()
{
    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    ADC_Configure();
    NVIC_Configure();

    LCD_Init();
    Touch_Configuration();
    Touch_Adjust();
    LCD_Clear(WHITE);

    uint16_t r = 5;

    while(1) {
        LCD_ShowString(LCD_NAME_POS_X, LCD_NAME_POS_Y, "TUE_TEAM06", BLACK, WHITE);

        if (!T_INT) {
            Touch_GetXY(&pos_x, &pos_y, 0);
            Convert_Pos(pos_x, pos_y, &cvt_pos_x, &cvt_pos_y);
            LCD_DrawCircle(cvt_pos_x, cvt_pos_y, r);
            LCD_ShowNum(LCD_LUXVAL_X, LCD_LUXVAL_Y, value, 5, BLACK, WHITE);
            LCD_ShowNum(LCD_X_POS_X, LCD_X_POS_Y, cvt_pos_x, 4, BLACK, WHITE);
            LCD_ShowNum(LCD_Y_POS_X, LCD_Y_POS_Y, cvt_pos_y, 4, BLACK, WHITE);
            delay();
        }
    }
}