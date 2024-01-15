// gpio.c
#include "gpio.h"

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x0000003B;     // 1) F clock  20 = 0011 1011 = A B - D E F
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x00;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}

void PortD_Init(void){
  GPIO_PORTD_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTD_CR_R = 0x0F;           // allow changes to PF4-0       
  GPIO_PORTD_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTD_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTD_DIR_R = 0x0F;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTD_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTD_PUR_R = 0x00;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTD_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}

void PortE_Init(void){
  GPIO_PORTE_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTE_CR_R = 0x0F;           // allow changes to PF4-0       
  GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0x00;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTE_PDR_R = 0x0F;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTE_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}

void PortA_Init(void){
  GPIO_PORTA_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTA_CR_R = 0x0F;           // allow changes to PF4-0       
  GPIO_PORTA_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTA_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTA_DIR_R = 0x0F;          // 5) PA3,PF2,PF1 output   
  GPIO_PORTA_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTA_PDR_R = 0x00;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTA_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}

void PortB_Init(void){
  GPIO_PORTB_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PB0  
  GPIO_PORTB_CR_R = 0x0F;           // allow changes to PB4-0       
  GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R = 0x0F;          // 5) PB3,PB2,PB1 and PB0 output   
  GPIO_PORTB_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTB_PUR_R = 0x00;          // enable pullup resistors on PB4,PB0       
  GPIO_PORTB_DEN_R = 0x0F;          // 7) enable digital pins PB4-PB0        
}
