/*
  Author: Justin Mendoza
  Assignment: Lab 2 Part D.
  Code for 10KHz Interrupts.
  Reference from Valvano's code and class textbook. 

*/

#include <CortexM.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/debug.h>
#include <driverlib/fpu.h>
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>
#include <driverlib/pin_map.h>
#include <driverlib/rom.h>
#include <driverlib/sysctl.h>
#include <driverlib/timer.h>
#include <driverlib/uart.h>
#include <utils/uartstdio.h>
#include <inc/tm4c123gh6pm.h>
#include <rom.h>


// Adderesses Declared Here
#define GPIO_PORTF_DIR_R        (*((volatile uint32_t *)0x40025400)) // These do not have to be defined, included in libaries.
#define GPIO_PORTF_DEN_R        (*((volatile uint32_t *)0x4002551C)) // These are here for reference. 
#define GPIO_PORTF_DATA_R       (*((volatile uint32_t *)0x400253FC))
#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))
#define RED (0x01 << 1) // LED Colors used for output.
#define BLUE (0x01 << 2)
#define GREEN (0x01 << 3)

// Delays n Milliseconds.
void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++) {}   /* do nothing for 1 ms */
}

// Init. PortF for use.
void initPortF()
{
    volatile uint32_t delay;
    SYSCTL_RCGC2_R |= 0x00000020;       /* enable clock to GPIOF */
    delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating
    GPIO_PORTF_DIR_R = 0x0E; // Set pins PF1,PF2,PF3 on TIVA board as output. 0b1110. 
    GPIO_PORTF_DEN_R = 0x0E; // Set Pins as digital pins. 
}

void Timer1_Init(void){ volatile uint32_t delay;
SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating
TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, down-count 
TIMER1_TAILR_R = 0xFFFFFFFF;  // 4) reload value
TIMER1_TAPR_R = 0;            // 5) bus clock resolution
TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
}

void Timer2A_Init(uint32_t period)
{
  	SYSCTL_RCGCTIMER_R|=0x04;       //0)    activatetimer2
        //someInterrupt=task;	//user function
        TIMER2_CTL_R	=	0x00000000;	//	1)      disable	TIMER2A	during	setup
        TIMER2_CFG_R	=	0x00000000;	//	2)	configure	for	32-bit	mode
       	TIMER2_TAMR_R	=	0x00000002;	//	3)	configure	for	periodic	mode         
       	TIMER2_TAILR_R	=	period-1;	//	4)	reload	value
        TIMER2_TAPR_R	=	0;	        //	5)	bus	clock	resolution
        TIMER2_ICR_R	=	0x00000001;				//	6)	clear	timer2A	timeout	flag
        TIMER2_IMR_R	=	0x00000001;				//	7)	arm	timeout	interrupt
        NVIC_PRI5_R	=	(NVIC_PRI5_R&0x00FFFFFF)|0x00000000;	//	8)	priority 1 interrupt
        NVIC_EN0_R	=	1<<23;					//	9)	enable	IRQ 23 in NVIC
        TIMER2_CTL_R	=	0x00000001;				//	10)	enable	timer2A
        EnableInterrupts();
}

void Timer2A_Handler(void)
{
  TIMER2_ICR_R	= 0x00000001;	//acknowledge timer2A timeout
  someInterrupt();// execute user task, Blue LED should be on very breifly. 
}

void someInterrupt()
{
        GPIO_PORTF_DATA_R = RED;    // Assign bits in PORTF so Red LED will turn on
}


int main()
{

    initPortF();
    Timer2A_Init(8000); // 10Khz Interrupt timer, period parameter value 8000. 
   
  while(1)
  {
    // Loop while creating interrupts generated from timer. 
  }
  return 0;
}


