/* 
 * File:   Maestro.c
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


//LIBRERIAS//
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPI.h"
#include "LCD.h"

//DEFINICIONES//
#define _XTAL_FREQ 4000000
#define RS RC0
#define EN RC1
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7
#define _tmr0_value 223

//VARIABLES//
char pot1, pot2;
char estado;

//PROTOTIPOS//
void setup(void);           //PIC configuracion
void ReadSlave(void);       //Read from slave

//INTERRUPCIONES//
void __interrupt() isr(void)
{
    
}

//MAIN//
void main(void)
{
    setup();
    Lcd_Init();
    Lcd_Clear();
    
    while(1){
        
        Lcd_Set_Cursor(1,1);        //En esta posición del LCD
		Lcd_Write_String("S1");     //Se escribe esto
		Lcd_Set_Cursor(1,8);
		Lcd_Write_String("S2");
        
        PORTCbits.RC2 = 0;      //Comunicacion con slave == 1
        __delay_ms(1);
        
        //En esta parte también se incluye lo del lab, en este caso
        //la diferencia que tiene del postlab es la varible qeu recibe el 
        //dato del esclavo
        
        estado = 0;
        spiWrite(estado);
        pot1 = spiRead();
        
        __delay_ms(100);
        
        estado = 1;
        spiWrite(estado);
        pot2 = spiRead();
        
        __delay_ms(1);
        PORTCbits.RC2 = 1;      //Comunicacion con slave == 0
        __delay_ms(250);
        
        
        Lcd_Set_Cursor(2,1);
		Lcd_Write_String(pot1);
		Lcd_Set_Cursor(2,5);
		Lcd_Write_String("V");

		Lcd_Set_Cursor(2,7);
		Lcd_Write_String(pot2);
		Lcd_Set_Cursor(2,11);
		Lcd_Write_String("V");

		__delay_ms(5000);
    
    }
    
}


void setup(void)
{
    ANSEL   = 0x00;               
    ANSELH  = 0x00;   
    
    TRISC   = 0x00;
    TRISD   = 0x00; 
    
    PORTC   = 0x00;
    PORTD   = 0x00;
    
    PORTCbits.RC2 = 1;
    
    INTCONbits.GIE = 1; //Interrupciones globales
    INTCONbits.PEIE = 1;
    PIR1bits.SSPIF = 1; //bandera SSPIF == 0
    PIE1bits.SSPIE = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    OSCCONbits.IRCF2    = 1;
    OSCCONbits.IRCF1    = 1;
    OSCCONbits.IRCF0    = 0;
    OSCCONbits.SCS      = 1; //Oscilador interno
    
}



