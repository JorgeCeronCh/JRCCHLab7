/*
 * File:   Lab7.c
 * Author: Jorge Cerón
 *
 * Created on 4 de abril de 2022, 10:20 PM
 */
// PIC16F887 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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
#include <stdint.h>             // int8_t, unit8_t
#define _tmr0_value 217         // Valor para 10 ms
#define _XTAL_FREQ 4000000

uint8_t cont1;
uint8_t cont2;

void tmr0_setup(void){
    OPTION_REGbits.T0CS = 0;     // Temporizador
    OPTION_REGbits.PSA = 0;      // Prescaler a TMR0
    OPTION_REGbits.PS0 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS2 = 1;      // Prescaler 1:256
    INTCONbits.T0IF = 0;         // Limpiar la bandera de TMR0
    TMR0 = _tmr0_value;          // Valor de retraso
    return;
}

void __interrupt() isr (void){
    if(INTCONbits.RBIF){        // Verificar si se activa la bandera de interrupcion de PORTB 
        if(!PORTBbits.RB0){     // Verificar si fue actividad por presionar RB0
            PORTA++;            // Incrementar en 1 PORTA 
        }
        else if(!PORTBbits.RB1){// Verificar si se activa la bandera de interrupcion de PORTB 
            PORTA--;            // Decrementar en 1 PORTA 
        }
        INTCONbits.RBIF = 0;    // Limpiar la bandera de interrupcion de PORTB
    }
    else if(INTCONbits.T0IF){   // Verificar si se activa la bandera de interrupcion de TMR0 
        cont1++;                // Incrementar en 1 cont1
        if(cont1 == 10){        // Verificar si se repite 10 cont1 para hacer 100 ms
            cont2++;            // Incrementar en 1 cont2
            cont1 = 0;          // Limpiar cont1
        }
        INTCONbits.T0IF = 0;// Limpiar la bandera de TMR0
        TMR0 = _tmr0_value; // Valor de retraso para reinicio de TMR0
    }
    return;
}

void setup(void){
    ANSEL = 0;
    ANSELH = 0;                 // I/O digitales
        
    TRISA = 0x00;               // PORTA como salida
    TRISC = 0x00;               // PORTC como salida
    PORTA = 0x00;               // Se limpia PORTA
    PORTC = 0x00;               // Se limpia PORTC
    
    TRISBbits.TRISB0 = 1;       // RBO como entrada
    TRISBbits.TRISB1 = 1;       // RB1 como entrada
    
    OPTION_REGbits.nRBPU = 0;   // Habilitar Pull-ups
    WPUBbits.WPUB0 = 1;         // Pull-up en RB0
    WPUBbits.WPUB1 = 1;         // Pull-up en RB0
    INTCONbits.GIE = 1;         // Habilitar interrupciones globales
    INTCONbits.T0IE = 1;        // Habilitar interrupciones de TMR0
    INTCONbits.RBIE = 0;        // Deshabilitar interrupciones de PORTB
    IOCBbits.IOCB0 = 1;         // Habilitar interrupciones de cambio de estado en RB0
    IOCBbits.IOCB1 = 1;         // Habilitar interrupciones de cambio de estado en RB1
    INTCONbits.RBIF = 0;        // Limpiar la bandera de interrupcion de PORTB
    INTCONbits.T0IF = 0;        // Limpiar la bandera de TMR0
    // Configuración del oscilador
    OSCCONbits.IRCF = 0b0110;   // 4MHz
    OSCCONbits.SCS = 1;         // Oscilador interno
    return;
}

void main(void){
    setup();
    tmr0_setup();
    while(1){                   // Ejecutar para siempre
        PORTC = cont2;          // Mostrar el valor de cont2 en PORTC
    }
    return;
}
