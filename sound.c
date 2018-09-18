/*
============
MODULE: sound.c

DESCRIPTION: complex sound player with dynamic frequency changer.

REMARKS: used as a base to generate the sound of a police car.
============
*/

#include <Arduino.h>
#include "beep.h"

static struct BEEP	mm_siren [] = {
	{200,	1,	BEEP_FIXED},
	{2000,	2500,	BEEP_SMOOTH},
	{200,	2500,	BEEP_SMOOTH},
	{1200,	120,	BEEP_SMOOTH},
	{200,	120,	BEEP_SMOOTH},
	{1200,	120,	BEEP_SMOOTH},
	{200,	120,	BEEP_SMOOTH},
	{1200,	120,	BEEP_SMOOTH},
	{200,	120,	BEEP_SMOOTH},
	{1200,	120,	BEEP_SMOOTH},
	{200,	120,	BEEP_SMOOTH},

	{1200,	120,	BEEP_SMOOTH},
	{200,	120,	BEEP_SMOOTH},
	{1200,	120,	BEEP_SMOOTH},
	{200,	120,	BEEP_SMOOTH},
	{1200,	120,	BEEP_SMOOTH},
	{200,	120,	BEEP_SMOOTH},
	{1200,	120,	BEEP_SMOOTH},
	{200,	120,	BEEP_SMOOTH},
};

static struct BEEP	mm_greet1 [] = {
	{400,	10,	BEEP_FIXED},
	{1200,	300,	BEEP_FIXED},
	{1500,	100,	BEEP_FIXED},
	{2200,	60,	BEEP_FIXED},
	{800,	120,	BEEP_FIXED},
	{1000,	120,	BEEP_FIXED},
	{1200,	220,	BEEP_FIXED},
	{500,	20,	BEEP_FIXED},
};

static struct BEEP	mm_greet [] = {
	{600,	100,	BEEP_FIXED},
	{1200,	100,	BEEP_FIXED},
	{1000,	100,	BEEP_FIXED},
	{1600,	100,	BEEP_FIXED},
	{800,	200,	BEEP_FIXED},
};

static struct BEEP	mm_klaxon [] = {	// —игнал клаксона служебного авто (повторить ? раз)
	{20,	10,	BEEP_FIXED},
	{30,	4,	BEEP_FIXED},
	{80,	2,	BEEP_FIXED},
};

void Sound_Greet (void)
{
	BEEP_melody (1, mm_greet, sizeof(mm_greet) / sizeof(mm_greet[0]));
}

void Sound_Siren (void)
{
	BEEP_melody (8, mm_siren, sizeof(mm_siren) / sizeof(mm_siren[0]));
}

void Sound_Klaxon (void)
{
	BEEP_melody (10, mm_klaxon, sizeof(mm_klaxon) / sizeof(mm_klaxon[0]));
}

