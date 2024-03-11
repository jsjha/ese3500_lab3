/*
 * PartE.c
 *
 * Created: 3/11/2024 12:36:29 PM
 * Author : jessijha
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>
#include "uart.h"

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#define __PRINT_NEW_LINE__  UART_putstring("\r\n");

char String[25];


void Initialize() {
    // Setup for ADC (10bit = 0-1023)
    // Clear power reduction bit for ADC
    PRR0 &= ~(1 << PRADC);

    // Select Vref = AVcc
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);

    // Set the ADC clock div by 128
    // 16M/128=125kHz
    ADCSRA |= (1 << ADPS0);
    ADCSRA |= (1 << ADPS1);
    ADCSRA |= (1 << ADPS2);

    // Select Channel ADC0 (pin C0)
    ADMUX &= ~(1 << MUX0);
    ADMUX &= ~(1 << MUX1);
    ADMUX &= ~(1 << MUX2);
    ADMUX &= ~(1 << MUX3);

    ADCSRA |= (1 << ADATE);   // Autotriggering of ADC

    // Free running mode ADTS[2:0] = 000
    ADCSRB &= ~(1 << ADTS0);
    ADCSRB &= ~(1 << ADTS1);
    ADCSRB &= ~(1 << ADTS2);

    // Disable digital input buffer on ADC pin
    DIDR0 |= (1 << ADC0D);

    // Enable ADC
    ADCSRA |= (1 << ADEN);

    // Start conversion
    ADCSRA |= (1 << ADSC);
}


int main(void) {
	UART_init(BAUD_PRESCALER);
	Initialize();
	 while (1) {
		 char intStringBuffer[10];         // Buffer to hold the converted number
		 itoa(ADC, intStringBuffer, 10);   // Convert integer to string
		 UART_putstring("ADC Read:\t");
		 UART_putstring(intStringBuffer);
		 __PRINT_NEW_LINE__ __PRINT_NEW_LINE__   // Make space between prints
		 _delay_ms(5000);
	 }
}

