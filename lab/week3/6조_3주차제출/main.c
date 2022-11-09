#include "stm32f10x.h"

#define RCC_APB2_ENR *(volatile unsigned int *)0x40021018
#define GPIOB_CRH *(volatile unsigned int *)0x40010C04
#define GPIOC_CRL *(volatile unsigned int *)0x40011000
#define GPIOD_CRL *(volatile unsigned int *)0x40011400
#define GPIOD_BSRR *(volatile unsigned int *)0x40011410
#define GPIOC_IDR *(volatile unsigned int *)0x40011008
#define GPIOB_IDR *(volatile unsigned int *)0x40010C08
#define GPIOD_ODR *(volatile unsigned int *)0x4001140C

int main() {
  
  RCC_APB2_ENR = 0x00000038; // IO Port B, C, D Enable
  
  GPIOB_CRH = 0x44444444; // GPIO B Reset
  GPIOC_CRL = 0x44444444; // GPIO C Reset
  GPIOD_CRL = 0x44444444; // GPIO D Reset
  
  GPIOB_CRH = 0x44444448; // PB8 Set
  GPIOC_CRL = 0x44888844; // PC2, PC3, PC4, PC5 Set
  GPIOD_CRL = 0x34433344; // PD2, PD3, PD4, PD7 Set
  GPIOD_BSRR = 0x00000000; // Port bit Reset
  
  int isSelect = 0;
  
  while (1) {
    
    if (GPIOC_IDR == 0x0000FFDF) { // UP
      GPIOD_BSRR |= 0x00000090; // PD4, PD7 LED ON
      isSelect = 0;
    }
    else if (GPIOC_IDR == 0x0000FFFB) { // DOWN
      GPIOD_BSRR |= 0x00900000; // PD4, PD7 LED OFF
      isSelect = 0;
    }
    else if (GPIOC_IDR == 0x0000FFF7) { // LEFT
      GPIOD_BSRR |= 0x0000000C; // PD2, PD3 LED ON
      isSelect = 0;
    }
    else if (GPIOC_IDR == 0x0000FFEF) { // RIGHT
      GPIOD_BSRR |= 0x000C0000; // PD2, PD3 LED OFF
      isSelect = 0;
    }
    else if ( (((~GPIOB_IDR) << 16) & 0x01000000) && !isSelect ) { // SELECT
      if (GPIOD_ODR & 0x00000004) { // IF PD2 LED ON
        GPIOD_BSRR |= 0x00040000; // PD2 LED OFF
      }
      else { // IF PD2 LED OFF
        GPIOD_BSRR |= 0x00000004; // PD2 LED ON
      }
        
      if (GPIOD_ODR & 0x00000008) { // IF PD3 LED ON
        GPIOD_BSRR |= 0x00080000; // PD3 LED OFF
      }
      else { // IF PD3 LED OFF
        GPIOD_BSRR |= 0x00000008; // PD3 LED ON
      }
        
      if (GPIOD_ODR & 0x00000010) { // IF PD4 LED ON
        GPIOD_BSRR |= 0x00100000; // PD4 LED OFF
      }
      else { // IF PD4 LED OFF
        GPIOD_BSRR |= 0x00000010; // PD4 LED ON
      }
        
      if (GPIOD_ODR & 0x00000080) { // IF PD7 LED ON
        GPIOD_BSRR |= 0x00800000; // PD7 LED OFF
      }
      else { // IF PD7 LED OFF
        GPIOD_BSRR |= 0x00000080; // PD7 LED ON
      }
      isSelect = 1;
    }
    
  }
  
  return 0;
}