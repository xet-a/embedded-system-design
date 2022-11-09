#include "stm32f10x.h"

#define RCC_APB2_ENR *(volatile unsigned int *)0x40021018
#define GPIOC_CRL *(volatile unsigned int *)0x40011000
#define GPIOE_CRL *(volatile unsigned int *)0x40011800
#define GPIOE_BSRR *(volatile unsigned int *)0x40011810
#define GPIOC_IDR *(volatile unsigned int *)0x40011008

int main() {    
  RCC_APB2_ENR = 0x00000050; // IO Port C, E Enable

  GPIOC_CRL = 0x44444444; // GPIO C Reset
  GPIOE_CRL = 0x44444444; // GPIO E Reset
  
  GPIOC_CRL = 0x44844844; // PC2, PC5 Set
  GPIOE_CRL = 0x44444344; // PE2 Set
  GPIOE_BSRR = 0x00000000; // Port bit Reset
  
  
  while (1) {
    
    if (GPIOC_IDR == 0x0000FFDF) { // UP
      GPIOE_BSRR = 0x00000004;
      for (int i=0; i<1000000; i++) {
        ;
      }
    }
    else if (GPIOC_IDR == 0x0000FFFB) { // DOWN
      GPIOE_BSRR = 0x00040000;
      for (int i=0; i<1000000; i++) {
        ;
      }
    }
    
  }
  
  return 0;
}