/*
 * PartB.c
 *
 * Created: 2/16/2024 2:20:23 PM
 * Author : jessijha
 */ 

#define F_CPU 16000000UL
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>
#include "uart.h"


#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#define __PRINT_NEW_LINE__  UART_putstring("\r\n");

char String[25];

void Initialize ()
{
	// Disable all global interrupts initially
	cli();
	
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
	
	// Set PB1 as output pin for the Buzzer
	DDRD |= (1 << DDD5);
	
	// Set clock source for Timer 0 with prescaler of 256
	TCCR0B |= (1 << CS02);
	
	// Set Timer 1 to PWM Phase Match
	TCCR0A |= (1 << WGM00);
	TCCR0B |= (1 << WGM02);
	
	// Toggle OC1A on Compare Match
	TCCR0A |= (1 << COM0A0);
	
	// Toggle OC1B on Compare Match
	TCCR0A |= (1 << COM0B1);
	
	// Uses the formula TOP = ((f_clock) / (2 * Prescaler * (f_desired))
	OCR0A = 26;
	
	// Sets Duty Cycle?
	OCR0B = OCR0A * .95;   // Sets duty cycle, 5%

	// Enable all global interrupts
	sei();
}


int main(void)
{
	UART_init(BAUD_PRESCALER);
	Initialize();
	while (1) {
		char intStringBuffer[10];         // Buffer to hold the converted number
		itoa(ADC, intStringBuffer, 10);   // Convert integer to string
		OCR0B = (int) OCR0A * ADC / (1023 * 2);
		
		UART_putstring("ADC Read:\t");
		UART_putstring(intStringBuffer);
		__PRINT_NEW_LINE__ __PRINT_NEW_LINE__   // Make space between prints
		//_delay_ms(5000);
	}
}


///*
 //* PartE.c
 //*
 //* Created: 3/11/2024 12:36:29 PM
 //* Author : jessijha
 //*/ 
//
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <math.h>
//#include <util/delay.h>
//#include "uart.h"
//
//#define F_CPU 16000000UL
//#define BAUD_RATE 9600
//#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
//#define __PRINT_NEW_LINE__  UART_putstring("\r\n");
//
//char String[25];
//
//
//void Initialize() {
	//
	//cli();   // Disable global interrupts
	//
    //// Setup for ADC (10bit = 0-1023)
    //// Clear power reduction bit for ADC
    //PRR0 &= ~(1 << PRADC);
//
    //// Select Vref = AVcc
    //ADMUX |= (1 << REFS0);
    //ADMUX &= ~(1 << REFS1);
//
    //// Set the ADC clock div by 128
    //// 16M/128=125kHz
    //ADCSRA |= (1 << ADPS0);
    //ADCSRA |= (1 << ADPS1);
    //ADCSRA |= (1 << ADPS2);
//
    //// Select Channel ADC0 (pin C0)
    //ADMUX &= ~(1 << MUX0);
    //ADMUX &= ~(1 << MUX1);
    //ADMUX &= ~(1 << MUX2);
    //ADMUX &= ~(1 << MUX3);
//
    //ADCSRA |= (1 << ADATE);   // Autotriggering of ADC
//
    //// Free running mode ADTS[2:0] = 000
    //ADCSRB &= ~(1 << ADTS0);
    //ADCSRB &= ~(1 << ADTS1);
    //ADCSRB &= ~(1 << ADTS2);
//
    //// Disable digital input buffer on ADC pin
    //DIDR0 |= (1 << ADC0D);
//
    //// Enable ADC
    //ADCSRA |= (1 << ADEN);
//
    //// Start conversion
    //ADCSRA |= (1 << ADSC);
//
	//// Set PD6 as output pin for the Speaker
	//DDRD |= (1 << DDD6);
//
	//// Set clock source for Timer 0 with prescaler of 256
	//TCCR0B |= (1 << CS02);
//
	////// Timer1, Fast PWM mode
	////TCCR0A |= (1 << WGM00);
	////TCCR0A |= (1 << WGM01);
	////TCCR0B |= (1 << WGM02);
	//
	//// Set Timer 1 to PWM Phase Correct
	//TCCR0A |= (1 << WGM00);
	//TCCR0B |= (1 << WGM02);
	//
//
	//OCR0A = 140;      // Sets frequency, 2093 Hz
	////OCR0B = OCR0A * .25;   // Sets duty cycle, 75%
//
	//// Non-inverting mode
	//// Clear on Compare Match
	//TCCR0A |= (1 << COM0B1);
//
	//sei();   // Enable global interrupts
//}
//
//
//int main(void) {
	//UART_init(BAUD_PRESCALER);
	//Initialize();
	 //while (1) {
		 ////char intStringBuffer[10];         // Buffer to hold the converted number
		 ////itoa(ADC, intStringBuffer, 10);   // Convert integer to string
		 //////OCR0B = (int) OCR0A * ADC / 959;
		 ////UART_putstring("ADC Read:\t");
		 ////UART_putstring(intStringBuffer);
		 ////__PRINT_NEW_LINE__ __PRINT_NEW_LINE__   // Make space between prints
		 ////_delay_ms(5000);
	 //}
//}

