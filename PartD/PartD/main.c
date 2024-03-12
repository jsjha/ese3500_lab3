/*
 * PartD.c
 *
 * Created: 3/10/2024 8:18:41 PM
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


volatile int isDiscrete = 0;
volatile int isReady = 1;
volatile unsigned int duration = 0;
volatile double distance = 0;
char String[25];


void Initialize()
{
	// Disable all global interrupts initially
	cli();
	
	// Set PB1 as output pin for the LED
	DDRD |= (1 << DDD6);
	
	// Set clock source for Timer 0 with prescaler of 256
	TCCR0B |= (1 << CS02);
	
	// Set Timer 1 to PWM Phase Match
	TCCR0A |= (1 << WGM00);
	TCCR0B |= (1 << WGM02);
	
	// Toggle OC1A on Compare Match
	TCCR0A |= (1 << COM0A0);
	
	// Uses the formula TOP = ((f_clock) / (2 * Prescaler * (f_desired))
	OCR0A = 29;

	// Set PB1 as the Output Pin (we will send a HIGH pulse to this pin for at least 10us every time we want to take a reading --> Trigger)
	DDRB |= (1 << DDB1);
	
	// Set PB0 as the Input Pin (this captures data from the Ultrasonic Sensor --> Echo)
	DDRB &= ~(1 << DDB0);
	
	// Set clock source for Timer 1 with prescaler of 1
	TCCR1B |= (1 << CS10);
	
	// Configure Timer 1 for input capture mode with rising and falling edge detection
	TCCR1B |= (1 << ICES1);    // Set to capture rising edge first
	TIFR1 |= (1 << ICF1);	   // Clears interrupt flag
	TIMSK1 |= (1 << ICIE1);    // Enable input capture interrupt
	
	// PB Interrupt
	
	DDRB &= ~(1 << DDB7);	 // Set PB7 as input
	//PORTB |= (1 << PORTB7);  // Enable pull-up resistor on PB7

	PCICR |= (1 << PCIE0);	 // Enable Pin Change Interrupt for PCINT0-7
	PCMSK0 |= (1 << PCINT7); // Enable Pin Change Interrupt for PB7
	
	DDRB |= (1 << DDB5);	 // Set PB5 as an output
	PORTB &= ~(1 << PORTB5);   // Turn off the User LED by default


	//// Enables Timer 1 Overflow Interrupt
	//TIMSK1 |= (1 << TOIE1);
	//TIFR1 |= (1<<TOV1);
	
	// Enable all global interrupts
	sei();
}

ISR(PCINT0_vect) {
	if ((PINB & (1 << PINB7))) {
		isDiscrete = !isDiscrete;
		PORTB ^= (1 << PORTB5);	// Toggle the LED
	}
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
		
		// Toggle input capture ISR to look for a low-to-high edge
		TCCR1B |= (1 << ICES1);
		
		// We are now "ready" to take another measurement
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
		
		//// Continue sending pulses to the trigger pin 
		if (isReady == 1) {
			// We will use this value to calculate the distance traveled
			// Formula: speed of sound in cm/s * prescaler * duration (in ticks) / 16 MHz
			
			distance = (343.0d * 100.0d * 1.0d * duration) / F_CPU / 2.0d;
			 
			 if (isDiscrete == 0) {
			
				// Set the OCR0A so that the frequency of the buzzer changes
				OCR0A = (int) (0.276*(distance) + 13.326);
	
			
			 } else {
				 if (distance < 9.26) {
					 OCR0A = 14;
					 } else if (distance >= 9.26 && distance < 16.05) {
					 OCR0A = 15;
					 } else if (distance >= 16.05 && distance < 22.84) {
					 OCR0A = 17;
					 } else if (distance >= 22.84 && distance < 29.63) {
					 OCR0A = 19;
					 } else if (distance >= 29.63 && distance < 36.42) {
					 OCR0A = 22;
					 } else if (distance >= 36.42 && distance < 43.21) {
					 OCR0A = 23;
					 } else if (distance >= 43.21 && distance < 50.0) {
					 OCR0A = 26;
					 } else if (distance >= 50.0 && distance <= 56.79) {
					 OCR0A = 29;
					 } else {
					 OCR0A = 14;
				}

			 }
			 
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


