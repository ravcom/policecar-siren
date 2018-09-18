/*
============
MODULE: timer.c

Timer registers:
 ASSR (Asyncrhonous Operation)
 TIMSK (Timer/Counter Interrupt Mask) 
 TIFR (Timer/Counter Interrupt Flag Register)
 TCCRn (Timer/Counter Control Register)
 TCNTn (Timer/COunter Register)
 OCRn (Ouput Compare Register)
============
*/

#include <avr/interrupt.h>
#include <Arduino.h>
#include "pins_arduino.h"
#include "timer.h"


/*
============
TIMER_start

Starts specified timer.

The timer2 is more precise than timer0.

The interrupt function should be declared for the desired timer as following:
TIMER_0		- ISR(TIMER0_COMPA_vect) { .. }, uses Arduino's pins 5, 6
TIMER_2		- ISR(TIMER2_COMPA_vect) { .. }, uses Arduino's pins 3, 11
============
*/

void TIMER_start (byte id)
{
	switch (id)
	{
	case TIMER_0:
	        TCCR0A = 1 << WGM01;	// CTC mode
	        TCCR0B = 0;		// No prescaling at init
		break;

	case TIMER_2:
	        TCCR2A = 1 << WGM21;	// CTC mode
	        TCCR2B = 0;		// No prescaling at init
		break;
	}
}

void TIMER_stop (byte id)
{
	switch (id)
	{
	case TIMER_0:
		TIMSK0 &= ~(1 << OCIE0A); // disable interrupt
		TCCR0A = 0;
		TCCR0B = 0;
		OCR0A = 0;
		break;
	case TIMER_2:
		TIMSK2 &= ~(1 << OCIE2A); // disable interrupt
		TCCR2A = 0;
		TCCR2B = 0;
		OCR2A = 0;
		break;
	}
}

/*
============
TIMER_set_frequency

Set timer frequency.

Important:  Appropriate timer vector function should be defined:
- timer 0: ISR(TIMER0_COMPA_vect) and ISR(TIMER0_COMPB_vect)
- timer 2: ISR(TIMER2_COMPA_vect) and ISR(TIMER2_COMPB_vect)
============
*/

void TIMER_set_frequency (byte id, unsigned int frequency)
{
	byte prescalarbits = 0b001;  // ck/1: same for both timers
	uint32_t period = F_CPU / (frequency * 2);
	uint32_t ocrA = period;
	uint16_t ocrB = 32;

		// if we are using an 8 bit timer, scan through prescalars to find the best fit
	switch (id)
	{
	case TIMER_0:
		if (ocrA > 256)
		{
			ocrA = period / 8;
			prescalarbits = 0b010;	// ck/8: same for both timers
		}
		if (ocrA > 256)
		{
			ocrA = period / 64;
			prescalarbits = 0b011;
		}
		if (ocrA > 256)
		{
			ocrA = period / 256;
			ocrB = 64;
			prescalarbits = 0b100;
		}
		if (ocrA > 256)
		{
			// can't do any better than /1024
			ocrA = period / 1024;
			ocrB = 16;
			prescalarbits = 0b101;
		}

		TCCR0B = (TCCR0B & 0b11111000) | prescalarbits;
		OCR0A = (ocrA - 1);				// Set timer A
		OCR0B = (ocrB - 1);				// Set timer B
		TIMSK0 |= (1 << OCIE0A) | (1 << OCIE0B);	// turn on the interrupts
		break;

	case TIMER_2:
		if (ocrA > 256)
		{
			ocrA = period / 8;
			prescalarbits = 0b010;	// ck/8: same for both timers
		}
		if (ocrA > 256)
		{
			ocrA = period / 32;
			prescalarbits = 0b011;
		}
		if (ocrA > 256)
		{
			ocrA = period / 64;
			ocrB = 8;
			prescalarbits = 0b100;
		}
		if (ocrA > 256)
		{
			ocrA = period / 128;
			ocrB = 4;
			prescalarbits = 0b101;
		}
		if (ocrA > 256)
		{
			ocrA = period / 256;
			ocrB = 2;
			prescalarbits = 0b110;
		}
		if (ocrA > 256)
		{
			ocrA = period / 1024;		// can't do any better than /1024
			ocrB = 1;
			prescalarbits = 0b111;
		}

		TCCR2B = (TCCR2B & 0b11111000) | prescalarbits;
		OCR2A = (ocrA - 1);				// Set time for timer A
		OCR2B = (ocrB - 1);				// Set time for timer B
		TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B);	// turn on the interrupts
		break;
	}
}
