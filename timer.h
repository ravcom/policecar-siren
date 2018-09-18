enum {TIMER_0, TIMER_1, TIMER_2};

void				TIMER_start (byte id);
void				TIMER_stop (byte id);
void				TIMER_set_frequency (byte id, unsigned int frequency);

