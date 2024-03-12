/*
 * PartB.c
 *
 * Created: 2/16/2024 2:20:23 PM
 * Author : jessijha
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
void Initialize ()
{
	// Disable all global interrupts initially
	cli();
	
	// Set PB1 as output pin for the Buzzer
	DDRD |= (1 << DDD6);
	
	// Set clock source for Timer 0 with prescaler of 256
	TCCR0B |= (1 << CS02);
	
	// Set Timer 1 to PWM Phase Match
	TCCR0A |= (1 << WGM00);
	TCCR0B |= (1 << WGM02);
	
	// Toggle OC1A on Compare Match
	TCCR0A |= (1 << COM0A0);
	
	// Uses the formula TOP = ((f_clock) / (2 * Prescaler * (f_desired))
	OCR0A = 140;

	// Enable all global interrupts
	sei();
}


int main(void)
{
	Initialize();
	while (1);
}

//void Initialize ()
//{
	//// Disable all global interrupts initially
	//cli();
	//
	//// Set PB1 as output pin for the LED
	//DDRD |= (1 << DDD6);
	//
	//// Set clock source for Timer 0 with prescaler of 256
	//TCCR0B |= (1 << CS02);
	//
	//// Set Timer 1 to CTC (Clear Timer on Compare Match)
	//TCCR0A = (1 << WGM01);
	//
	//// Toggle OC1A on Compare Match
	//TCCR0A |= (1 << COM0A0);
	//
	//// Uses the formula OCRnA = ((f_clock) / (2 * Prescaler * (f_desired)) - 1)
	//OCR0A = 71;
//
	//// Enable all global interrupts
	//sei();
//}
//
//
//int main(void)
//{
	//Initialize();
	//while (1);
//}



//void Initialize )(
//{
	//// Disable all global interrupts initially
	//cli();
	//
	////// Sets Normal Mode??
	////TIFR0 |= (1<<TOV0);
	//
	//// Set PB1 as output pin for the LED
	//DDRD |= (1 << DDD6);
	////PORTD |= (1 << PORTD6);
	//
	//// Enables Timer 0 Overflow Interrupt
	//TIMSK0 |= (1 << TOIE0);
	//
	//// Enables Timer 0 Compare Interrupt
	//TIMSK0 |= (1 << OCIE0A);
	//
	//// Set clock source for Timer 0 with prescaler of 256
	//TCCR0B |= (1 << CS02);
	//
	//// Uses the formula f_generated = (f_clock) / (2 * Prescaler * (1 + OCRnA))
	//OCR0A = 71;
//
	//// Enable all global interrupts
	//sei();
//}
//
////ISR(TIMER0_OVF_vect) {
	////// Play (new?) note every time the timer overflows
	////PORTD ^= (1 << PORTD6);
////}
//
//ISR(TIMER0_COMPA_vect) {
	//OCR0A += 71;
	//PORTD ^= (1 << PORTD6);
//}
//
//
//
//int main(void)
//{
	//Initialize();
    //while (1);
//}

