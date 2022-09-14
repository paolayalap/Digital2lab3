/* 
 * File:   Esclavo.c
 * Author: paola
 *
 * Created on 12 de septiembre de 2022, 15:00
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
//LIBRERIAS//
#define _XTAL_FREQ 4000000

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ADC.h"
#include "SPI.h"

//VARIABLES//
uint8_t varADC1;
uint8_t varADC2;
int var1, var2;
char buffer[64];
float voltaje;



//PROTOTIPOS//
void setup(void);   //PIC configuraciones

//INTERRUPCIONES//
void __interrupt() isr(void)
{
    if(PIR1bits.ADIF == 1){
        if(ADCON0bits.CHS == 0){
            varADC1 = ADRESH;
            ADCON0bits.CHS = 1;
            
        }
        else if(ADCON0bits.CHS == 1){
            ADCON0bits.CHS = 0;
            varADC2 = ADRESH;
            
        }
        PIR1bits.ADIF = 0;
    }
    
    if(SSPIF == 1)
    {
        var1 = spiRead();
        spiWrite(varADC2);
        
        var2 = spiRead();
        spiWrite(varADC1);
        SSPIF = 0;
    }
}

//MAIN//
void main(void)
{
    setup();
    adc_init(32,0,0);
    
    while(1){
    
        if(ADCON0bits.GO == 0){
            adc_start(0);
            __delay_us(100);
            ADCON0bits.GO = 1;
            adc_start(1);
        }
    }
}

void setup(void)
{
    ANSEL = 0b00000011;
    ANSELH = 0;
    
    TRISA = 0X03;
    TRISB = 0x00;
    TRISC = 0x00;
    TRISD = 0x00;
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    
    OSCCONbits.IRCF2    = 1; //4MHz
    OSCCONbits.IRCF1    = 1;
    OSCCONbits.IRCF0    = 0;
    OSCCONbits.SCS      = 1; //Oscilador interno
    
    INTCONbits.GIE = 1; //Interrupciones globales
    INTCONbits.PEIE = 1;
    PIR1bits.SSPIF = 0; //bandera SSPIF == 0
    PIE1bits.SSPIE = 1;
    TRISAbits.TRISA5 = 1;
    
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
}
