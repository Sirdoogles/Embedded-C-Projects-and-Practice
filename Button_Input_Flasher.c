
// LEDS will flash when a button is held. For the TM4C123GH6PM Eval board, there is a built-in LED for pins PF1,PF2,PF3.
// LEDs will light when these pins are a logic high. Switches SW1 & SW2 are linked to pins PF4 & PF0, in respect. Switch will be active low. 
// Functions are used to easier readibility. 

 #include <stdint.h>
 #include <tm4c123gh6pm.h>
 #define RED (0x01 << 1)
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
    SYSCTL_RCGC2_R |= 0x00000020;       /* enable clock to GPIOF */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     /* unlock commit register, needed for SW1 & SW2*/
    GPIO_PORTF_CR_R = 0x01;             /* make PORTF0 configurable */
    GPIO_PORTF_DIR_R = 0x0E;            /* set PORTF1 pin as output (LED Red, Green, and Blue) pin */
    GPIO_PORTF_DEN_R = 0x1F;            /* set all PORTF as digital pins */
    GPIO_PORTF_PUR_R = 0x11;            /* 0b10001, enable pull up resistors for Pin 0 (SW2) and Pin 4 (SW1)*/  
}
// Function flashes led once. 
void flashLED(int color)
{
            GPIO_PORTF_DATA_R = color; // Turn on LED color.
            delayMs(1000); // 1 Sec Delay.
            GPIO_PORTF_DATA_R = 0; // Clear PORTF Data
            delayMs(1000);
            GPIO_PORTF_DATA_R = color;
}

int main(void)
{
    initPortF(); // Init. PortF for use. 
  
    while(1) {
        // LEDS will flash when button(s) are held down. 
        switch(GPIO_PORTF_DATA_R & 0x11)
        {
          // If Both buttons are pushed.
          case 0x00:
            flashLED(BLUE);
            break;
            
          // When SW1 is pushed.  
          case 0x01:
            flashLED(RED);
            break;
            
          // When SW2 is pushed.
          case 0x10:
            flashLED(GREEN);
            break;
        }
    }
    return 0;
}
