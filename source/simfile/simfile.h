#ifndef __UTASM_SIMFILE_SIMFILE_H__
#define __UTASM_SIMFILE_SIMFILE_H__

#include <util/vector.h>

#include <string.h>

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

/* Simfile enums */

/*
 * Meta-function template macro.
 * Don't let this go outside of this file's scope.
 */
#define string_to_generic(X, Y) static inline simfile_##X##_t string_to_##X(const char* str) \
{ \
	for (int i = 0; i < NUM_##Y; i++) \
	{ \
		if (!strcmp(X##_to_string(i), str)) return i; \
	} \
	return Y##_UNKNOWN; \
}

/* simfile_difficulty_t */

typedef enum
{
	DIFF_UNKNOWN,

	DIFF_BEGINNER,
	DIFF_EASY,
	DIFF_MEDIUM,
	DIFF_HARD,
	DIFF_CHALLENGE,
	DIFF_EDIT,

	NUM_DIFF
} simfile_difficulty_t;

static const char* difficulty_strings[NUM_DIFF] =
{
	"unknown"

	"beginner",
	"easy",
	"medium",
	"hard",
	"challenge",
	"edit"
};

#define difficulty_to_string(X) difficulty_strings[X]
string_to_generic(difficulty, DIFF);

/* simfile_event_type_t */

typedef enum
{
	EVENT_UNKNOWN,

	EVENT_BPM,
	EVENT_STOP,

	NUM_EVENT
} simfile_event_type_t;

static const char* event_type_strings[NUM_EVENT] =
{
	"unknown",

	"bpm",
	"stop",
};

#define event_type_to_string(X) event_type_strings[X]
string_to_generic(event_type, EVENT);

/* simfile_note_t */

typedef enum
{
	NOTE_UNKNOWN,

	// worlds funniest redefinition.
	NOTE_NONE = 0,

	/* Single note types. */
	NOTE_NORMAL,
	NOTE_FAKE,
	NOTE_LIFT,
	NOTE_MINE,

	/* Long note types. */
	NOTE_LONG,
	NOTE_ROLL,
	NOTE_END,

	NUM_NOTE
} simfile_note_t;

static const char* note_strings[NUM_NOTE] =
{
	"none",

	"tap"
	"fake",
	"lift",
	"mine",

	"long",
	"roll",
	"end"
};

#define note_to_string(X) note_strings[X]
string_to_generic(note, NOTE);

/* simfile_style_t */

typedef enum
{
	STYLE_UNKNOWN,

	STYLE_DANCE,
	STYLE_KB7,
	STYLE_POPN,
	STYLE_PUMP,
	STYLE_CUSTOM,

	NUM_STYLE
} simfile_style_t;

static const char* style_strings[NUM_STYLE] =
{
	"unknown",

	"dance",
	"kb7",
	"popn",
	"pump",
	"custom",
};

#define style_to_string(X) style_strings[X]
string_to_generic(style, STYLE);

#undef string_to_generic

/* Simfile structures */

typedef union
{
	simfile_bpm_event_t bpm;
	simfile_stop_event_t stop;
} simfile_generic_event_t;

typedef struct
{
	uint8_t size;
	uint16_t data;
} simfile_row_t;

typedef struct
{
	uint8_t key_count;

	simfile_style_t style;

	int32_t meter;
	simfile_difficulty_t difficulty;

	vector(simfile_bpm_event_t) bpms;
	vector(simfile_stop_event_t) stops;

	vec_uint32_t rows;
} simfile_chart_t;

typedef struct
{
	const char* name;
	const char* subtitle;
	const char* artist;
	const char* author;

	simfile_chart_t* charts;
} simfile_t;

simfile_t* simfile_create();
simfile_t* simfile_load(char* path);

void simfile_export(simfile_t* simfile, char* path);

void simfile_destroy(simfile_t* simfile);

void simfile_add_event(simfile_chart_t* file, simfile_event_type_t type, simfile_generic_event_t event);

#endif//__UTASM_SIMFILE_SIMFILE_H__
