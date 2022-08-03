 /*
 * Archivo         : spi.c
 * Autor           : Ligo George
 * Compañia        : electroSome
 * Proyecto        : SPI Library for MPLAB XC8
 * Microcontrolador: PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 * Modificado      : Paola Ayala Pineda
 */

#include "SPI.h"
#define FLAG_SPI 0X00
#define _XTAL_FREQ 1000000

//Variables
char cont_m = 0;
char cont_s = 0X00;


void spiInit(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge)
{   /*
     *Estos datos serán usados para la inicialización del SPI
     */
    TRISC5 = 0;         
    if(sType & 0b00000100)
    {
        SSPSTAT = sTransmitEdge;
        TRISC3 = 1;
    }
    else
    {
        SSPSTAT = sDataSample | sTransmitEdge;
        TRISC3 = 0;
    }
    
    SSPCON = sType | sClockIdle;
}

static void spiReceiveWait()
{
    while ( !SSPSTATbits.BF );
    PORTB = cont_m; //se muestra en el PORTB
    cont_m++;
    PORTAbits.RA7 = 1; //se deshabilita el ss del esclavo
    __delay_ms(10); //se espera para que el pic detecte el cambio en el pin
    PORTAbits.RA7 = 0; //habilita nuevamente el esclavo
    SSPBUF = FLAG_SPI; //se envía cualuquier dato, lo importante es que genere un pulso de reloj 
                        //para transmitir datos
    while( !SSPSTATbits.BF );
    PORTD = SSPBUF; //se muestra el dato recibido en el PORTD
    __delay_ms(1000); //se envía el dato y se espera 1 segundo
}

void spiWrite(char cont_m)
{
    SSPBUF = cont_m; //el valor se carga al contador del buffer
}

unsigned spiDataReady()
{
    if(SSPSTATbits.BF)
        return 1;
    else
        return 0;
}

char spiRead()
{
    spiReceiveWait(); 
    return(SSPBUF);
}



