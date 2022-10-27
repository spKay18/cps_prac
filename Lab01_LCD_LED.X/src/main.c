#include <xc.h>
#include <p33Fxxxx.h>
//do not change the order of the following 2 definitions
#define FCY 12800000UL 
#include <libpic30.h>

#include "lcd.h"
#include "led.h"
//#include "labs/include/types.h"

/* Configuration of the Chip */
// Initial Oscillator Source Selection = Primary (XT, HS, EC) Oscillator with PLL
#pragma config FNOSC = PRIPLL
// Primary Oscillator Mode Select = XT Crystal Oscillator mode
#pragma config POSCMD = XT
// Watchdog Timer Enable = Watchdog Timer enabled/disabled by user software
// (LPRC can be disabled by clearing the SWDTEN bit in the RCON register)
#pragma config FWDTEN = OFF


// display the least significant bits of the num
void LED_set(uint32_t num){
    uint32_t decider = num;
    decider = num & BV(0);
    if(decider == 0)
        CLEARLED(LED5_PORT);
    else
        SETLED(LED5_PORT);
    
    decider = num;
    decider = num & BV(1);
    if(decider == 0)
        CLEARLED(LED4_PORT);
    else
        SETLED(LED4_PORT);
    
    decider = num;
    decider = num & BV(2);
    if(decider == 0)
        CLEARLED(LED3_PORT);
    else
        SETLED(LED3_PORT);
    
    decider = num;
    decider = num & BV(3);
    if(decider == 0)
        CLEARLED(LED2_PORT);
    else
        SETLED(LED2_PORT);
    
    decider = num;
    decider = num & BV(4);
    if(decider == 0)
        CLEARLED(LED1_PORT);
    else
        SETLED(LED1_PORT);
}

int main(){
    //Init LCD and LEDs
    lcd_initialize();
    led_init();
	
    // Clear the Screen and reset the cursor
    lcd_clear();
    lcd_locate(0, 0);
    
    // Print Hello World
    lcd_printf("Group Members:");
    lcd_locate(0,1);
    lcd_printf("* Bo He");
    lcd_locate(0,2);
    lcd_printf("* Xiongzun Zhou");
    lcd_locate(0,3);
    lcd_printf("* Ke Wang");
    lcd_locate(0,7);
    lcd_printf("Counter = ");
    
    uint32_t count = 0;
    
    
    
    // Stop
    while(1){
        lcd_printf("%lu", count);
//        printf("%u", count);
        lcd_locate(10,7);       
        LED_set(count);
        __delay_ms(1000);
        ++count;
//        CLEARLED(LED1_TRIS); // set Pin to Output
//        CLEARLED(LED2_TRIS); // set Pin to Output
//        CLEARLED(LED3_TRIS); // set Pin to Output
//        CLEARLED(LED4_TRIS); // set Pin to Output
//        CLEARLED(LED5_TRIS); // set Pin to Output
//        
//        SETLED(LED1_PORT); // turn on LED
//        SETLED(LED2_PORT); // turn on LED
//        SETLED(LED3_PORT); // turn on LED
//        SETLED(LED4_PORT); // turn on LED
//        SETLED(LED5_PORT); // turn on LED
    }
        ;
}

