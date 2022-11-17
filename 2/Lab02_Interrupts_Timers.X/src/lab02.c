#include "lab02.h"

#include <xc.h>
#include <p33Fxxxx.h>
//do not change the order of the following 2 definitions
#define FCY 12800000UL
#include <libpic30.h>

#include "types.h"
#include "lcd.h"
#include "led.h"

#define FCY_EXT 32768

volatile uint16_t seconds = 0;
volatile uint16_t milliseconds = 0;

void initialize_timer()
{
    // Enable RTC Oscillator -> this effectively does OSCCONbits.LPOSCEN = 1
    // but the OSCCON register is lock protected. That means you would have to 
    // write a specific sequence of numbers to the register OSCCONL. After that 
    // the write access to OSCCONL will be enabled for one instruction cycle.
    // The function __builtin_write_OSCCONL(val) does the unlocking sequence and
    // afterwards writes the value val to that register. (OSCCONL represents the
    // lower 8 bits of the register OSCCON)
    __builtin_write_OSCCONL(OSCCONL | 2);
    
    //Timer1 setup
    //disable
    CLEARBIT(T1CONbits.TON);
    //1:256 prescaler
    T1CONbits.TCKPS = 0b11;
    // T1 external clock
    T1CONbits.TCS = 1;
    // Set Gated Timer Mode -> don't use gating
    CLEARBIT(T1CONbits.TGATE);
    // T1: Set External Clock Input Synchronization -> no sync
    T1CONbits.TSYNC = 0;
    // Load Timer Periods, 1s
    PR1 = 128;
    // Reset Timer Values
    TMR1 = 0x00;
    // Set Interrupt Priority
    IPC0bits.T1IP = 0x01;
    // clear interrupt flag
    IFS0bits.T1IF = 0;
    // enable interrupt
    IEC0bits.T1IE = 1;
    // start the timer
    T1CONbits.TON = 1;
    
    // timer 2 setup
    // disable
    CLEARBIT(T2CONbits.TON);
    // data sheet reference 1:256 prescaler "11"
    T2CONbits.TCKPS = 0b11;
    // internal instruction clock
    CLEARBIT(T2CONbits.TCS);
    //disable gate
    CLEARBIT(T2CONbits.TGATE);
    //set period, 1ms
    PR2 = 100;
    // reset value 
    TMR2 = 0x00;
    // interrupt priority
    IPC1bits.T2IP = 0x01;
    // clear interrupt flag
    CLEARBIT(IFS0bits.T2IF);
    // enable interrupt
    SETBIT(IEC0bits.T2IE);
    // enable timer
    SETBIT(T2CONbits.TON);
    
    // timer 3 setup
    // Disable the Timers
    CLEARBIT(T3CONbits.TON);
    // Set Prescaler
    // data sheet reference 1:1 prescaler "00"
    T3CONbits.TCKPS = 0b00;
    //internal instruction clock
    CLEARBIT(T3CONbits.TCS);
    // disable gate
    CLEARBIT(T3CONbits.TGATE);
    // largest period
    PR3 = 65535;
    // enable T3 timer
    SETBIT(T3CONbits.TON);
}

void timer_loop()
{
    uint16_t iterations = 0;
    
    // print assignment information
    lcd_printf("Lab02: Int & Timer");
    lcd_locate(0, 1);
    lcd_printf("Group: GroupName 3-6");
    
    TMR3 = 0x00;
    uint16_t tmr3 = TMR3;
    while(TRUE)
    {
        ++iterations;
        if(iterations == 2000) {        
        lcd_locate(0, 6);
        lcd_printf("%02u:%02u:%03u", seconds / 60, seconds % 60, milliseconds % 1000);
        lcd_locate(0, 7);
        tmr3 = TMR3;
        lcd_printf("c=%u, ", tmr3);
        lcd_printf("d=%.4fms", (tmr3 * PR3) / (float) 12800000 * 1000);
        TMR3 = 0x00;
        iterations = 0;
        TOGGLELED(LED3_PORT);
        Nop();
        }
    }
}

void __attribute__((__interrupt__, __shadow__, __auto_psv__)) _T1Interrupt(void)
{ // invoked every ??
    ++seconds;
    TOGGLELED(LED2_PORT);

    IFS0bits.T1IF = 0;
}

void __attribute__((__interrupt__, __shadow__, __auto_psv__)) _T2Interrupt(void)
{ // invoked every ??
    milliseconds += 2;
    TOGGLELED(LED1_PORT);
    if(milliseconds > 1000)
        milliseconds = 0;
    IFS0bits.T2IF = 0;
}
