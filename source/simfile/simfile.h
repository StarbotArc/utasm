#ifndef __UTASM_SIMFILE_SIMFILE_H__
#define __UTASM_SIMFILE_SIMFILE_H__

#include <util/vector.h>

/*
 * Meta-structure template macro functions for utility purposes.
 * Do not let these macro functions go outside the scope of this file.
 */

#define event(X, ...) typedef struct { \
	__VA_ARGS__ \
} simfile_##X##_event_t;

#define parameter(T, X) T X;

event(bpm, parameter(float, value))
event(stop, parameter(float, value))

#undef event
#undef parameter

typedef enum
{
	EVENT_BPM,
	EVENT_STOP,
} simfile_event_type_t;

typedef enum
{
	DIFF_BEGINNER,
	DIFF_EASY,
	DIFF_MEDIUM,
	DIFF_HARD,
	DIFF_CHALLENGE,
	DIFF_EDIT,
} simfile_difficulty_t;

typedef union
{
	simfile_bpm_event_t bpm;
	simfile_stop_event_t stop;
} simfile_event_generic_t;

typedef struct
{
	uint8_t size;
	uint16_t data;
} simfile_row_t;

typedef struct
{
	uint8_t key_count;

	int32_t meter;
	simfile_difficulty_t difficulty;

	vector(simfile_bpm_event_t) bpms;
	vector(simfile_stop_event_t) stops;

	vector(uint32_t) rows;
} simfile_chart_t;

typedef struct
{
	char* name;
	char* subtitle;
	char* artist;
	char* author;

	simfile_chart_t* charts;
} simfile_t;

simfile_t* simfile_create();
simfile_t* simfile_load(char* path);

void simfile_export(simfile_t* simfile, char* path);

void simfile_destroy(simfile_t* simfile);

void simfile_add_event(simfile_chart_t* file, simfile_event_type_t type, simfile_event_generic_t event);

#endif//__UTASM_SIMFILE_SIMFILE_H__
