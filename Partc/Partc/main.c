/*
 * Partc.c
 *
 * Created: 2/23/2024 2:35:13 PM
 * Author : jessijha
 */ 


#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>
#include "uart.h"

volatile int isReady = 1;
volatile unsigned int duration = 0;
volatile double distance = 0;
char String[25];

void Initialize()
{
	// Disable all global interrupts initially
	cli();

	// Set PD6 as the Output Pin (we will send a HIGH pulse to this pin for at least 10us every time we want to take a reading --> Trigger)
	DDRB |= (1 << DDB1);
	
	// Set PD7 as the Input Pin (this captures data from the Ultrasonic Sensor --> Echo)
	DDRB &= ~(1 << DDB0);
	
	// Set clock source for Timer 1 with prescaler of 1
	TCCR1B |= (1 << CS10);
	
	// Configure Timer 1 for input capture mode with rising and falling edge detection
	TCCR1B |= (1 << ICES1);    // Set to capture rising edge first
	TIFR1 |= (1 << ICF1);	   // Clears interrupt flag
	TIMSK1 |= (1 << ICIE1);    // Enable input capture interrupt

	//// Enables Timer 1 Overflow Interrupt
	//TIMSK1 |= (1 << TOIE1);
	//TIFR1 |= (1<<TOV1);
	
	// Enable all global interrupts
	sei();
}

// Interrupt Service Routine for Timer 1 input capture event
ISR(TIMER1_CAPT_vect)
{

	
	// Check to see if we are on low-to-high
	if (TCCR1B & (1<<ICES1)) {
		
		// Toggle input capture ISR to look for a high-to-low edge
		TCCR1B &= ~(1 << ICES1);
		
		// If so, set TCNT1 = 0 (this is the starting value for our duration count)
		TCNT1 = 0;
	
		
	} else {  // We are on a high-to-low edge
		
		// We set the ICR1 bit equal to the TCNT1 (ISR1 is more accurate)
		duration = ICR1;
		
		// We will use this value to calculate the distance traveled
		// Formula: speed of sound in cm/s * prescaler * duration (in ticks) / 16 MHz
		distance = (343.0d * 100.0d * 1.0d * duration) / F_CPU / 2.0d;
		
		// Print Distance to Serial Monitor
		//sprintf(String,"Distance = %.2f \r\n", distance);
		//UART_putstring(String);
		
		// Toggle input capture ISR to look for a low-to-high edge
		TCCR1B |= (1 << ICES1);
		
		//// We are now "ready" to take another measurement
		isReady = 1;
	}
}


int main(void)
{
	UART_init(BAUD_PRESCALER);
	
	Initialize();
	
	PORTB |= (1 << PORTB1);
	_delay_us(10);
	PORTB &= ~(1 << PORTB1);
	
	while (1) {
		
		//PORTB |= (1 << PORTB1);
		//_delay_us(10);
		//PORTB &= ~(1 << PORTB1);
		//_delay_ms(50);
		
		//// Continue sending pulses to the trigger pin
		if (isReady == 1) {
			// Print Distance to Serial Monitor
			sprintf(String,"Distance = %.2f \r\n", distance);
			UART_putstring(String);
			_delay_ms(5);
			PORTB |= (1 << PORTB1);
			_delay_us(10);
			PORTB &= ~(1 << PORTB1);
			
			isReady = 0;
		}
	}
}



