/*
=============
MODULE: siren.ino

AUTHOR: Sapunov Vladimir, warnash.com

DESCRIPTION: Makes an asynchronous police car sound.

REMARKS: BEEP_PIN and BEEP_PORT are defined in beep.c.

WARNING: Pins 3 and 11 should not be used as PWM output, because PWM module uses TIMER2 for them,
but we use this timer for the BEEP function. All other timers are also used by other PWM pins.
=============
*/

#include <Arduino.h>
#include <FileIO.h>
#include "D:\VHome\Source\Arduino\lib\ibus.cpp"

// These modules could be easily converted to *.CPP, but I prefer them in C lang.
extern "C" {
#include "timer.h"
#include "beep.h"
#include "sound.h"
}


void setup ()
{

	BEEP_init ();
	Sound_Greet ();

	//Sound_Siren ();
	//Sound_Klaxon ();
	//TIMER_set_frequency (2, 100);
}

void loop ()
{
	Sound_Siren ();
	delay (10 * 1000);
	BEEP_noTone ();
	delay (1000);

}

