#include <Arduino.h>
#include "timer.h"
#include "beep.h"

#define SMOOTH_PERIOD		10
#define BEEP_TIMER		2
#define BEEP_PIN		13 	/* Also defined in PORTB:5 */
#define BEEP_PORT		PORTB
#define BEEP_MASK		0b100000

static long			mm_toggle_count;
static word			mm_frequency;
static volatile word		mm_melodyRepeat;
static struct BEEP*		mm_melody;
static struct BEEP*		mm_melodyEnd;
static struct BEEP*		mm_tone;
static short			mm_smoothStep;



	// frequency (in hertz) and duration (in milliseconds).
void BEEP_init (void)
{
	pinMode (BEEP_PIN, OUTPUT);	// Set the pinMode as OUTPUT

	TIMER_start (BEEP_TIMER);
}

void BEEP_tone (unsigned int frequency, unsigned long duration)
{
		// Calculate the toggle count
	if (duration > 0) {
		mm_toggle_count = 2 * frequency * duration / 1000;
	} else {
		mm_toggle_count = 0;
	}

	TIMER_set_frequency (BEEP_TIMER, frequency);
}

void BEEP_noTone (void)
{
// TODO: volatile and interrupts disable to trustfully switching off the speaker
	mm_melodyRepeat = 0;
	BEEP_PORT &= ~BEEP_MASK;
}

void BEEP_melody (int repeat, struct BEEP melody[], size_t len)
{
	mm_melody = melody;
	mm_melodyEnd = mm_melody + len;
	mm_melodyRepeat = repeat;
	mm_tone = mm_melody;

	mm_frequency = mm_tone->frequency;

	BEEP_fetch ();
}

void BEEP_fetch (void)
{
	switch (mm_tone->type)
	{
	case BEEP_FIXED:
		BEEP_tone (mm_tone->frequency, mm_tone->duration);
		break;

	case BEEP_SMOOTH: {
		int	stepNum = mm_tone->duration / SMOOTH_PERIOD;
		mm_smoothStep = ((int)mm_tone->frequency - (int)mm_frequency) / stepNum;
		BEEP_tone (mm_frequency, SMOOTH_PERIOD);
		break; }
	}
}

boolean BEEP_advance (void)
{
	if ( ++mm_tone == mm_melodyEnd)
	{
		if (mm_melodyRepeat && --mm_melodyRepeat == 0)
		{
			BEEP_noTone ();
			return false;
		}
		mm_tone = mm_melody;
	}
	return true;
}

	// Manual toggle
void BEEP_toggle (void)
{
	BEEP_PORT ^= BEEP_MASK;
}

#if BEEP_TIMER == 0
	ISR(TIMER0_COMPA_vect)
#elif BEEP_TIMER == 1
	ISR(TIMER1_COMPA_vect)
#elif BEEP_TIMER == 2
	ISR(TIMER2_COMPA_vect)
#endif
{
	if (!mm_melodyRepeat)
		return;

	BEEP_PORT ^= BEEP_MASK;

	if (mm_toggle_count) {
		mm_toggle_count--;

	} else {
		switch (mm_tone->type)
		{
		case BEEP_FIXED:
			if (!BEEP_advance ())
				return;

			BEEP_fetch ();
			break;

		case BEEP_SMOOTH:
			if (mm_frequency == mm_tone->frequency)
			{
				if (!BEEP_advance ())
					return;

				BEEP_fetch ();
			} else {
				mm_frequency += mm_smoothStep;

				if ((mm_smoothStep > 0) == (mm_frequency > mm_tone->frequency))
				{
					mm_frequency = mm_tone->frequency;	// Cut if out of range
				}

				BEEP_tone (mm_frequency, SMOOTH_PERIOD);
			}
			break;
		}
	}
}
