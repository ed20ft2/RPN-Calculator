// systick.h
#ifndef SYSTICK_H
#define SYSTICK_H

//SysTick related Defines	
#define NVIC_ST_CTRL_R        (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R      (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R     (*((volatile unsigned long *)0xE000E018))


void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);

#endif // SYSTICK_H

