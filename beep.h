
enum {BEEP_FIXED, BEEP_SMOOTH};

struct BEEP {
	word		frequency;	// Final frequency from the current
	word		duration;	// Duration of a single fragment in millis
	byte		type;		// Type of beep: BEEP_INIT, BEEP_SMOOTH, BEEP_EXP_SMOOTH
};


void				BEEP_init (void);
void				BEEP_toggle (void);		// Ручное включение динамика
void				BEEP_noTone (void);
void				BEEP_tone (unsigned int frequency, unsigned long duration);
void				BEEP_melody (int repeat, struct BEEP beeps[], size_t count);
boolean				BEEP_advance (void);
void				BEEP_fetch (void);

