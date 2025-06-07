#ifndef __UTASM_SIMFILE_SIMFILE_H__
#define __UTASM_SIMFILE_SIMFILE_H__

#include <stdint.h>

#define list simfile_list_t

typedef struct
{
	float beat; 
	union
	{
		float f;
		const char* str;
	} value;
} simfile_event_t;

typedef union
{
	simfile_event_t event;
	uint32_t notedata;
} simfile_item_t;

typedef struct
{
	uint64_t size;
	simfile_item_t* items;
} simfile_list_t;

typedef struct
{
	uint8_t size;
	uint16_t data;
} simfile_row_t;

typedef struct
{
	char* name;
	char* subtitle;
	char* artist;
	char* author;

	list bpms;
	list stops;

	uint8_t key_count;
	list rows;
} simfile_t;

#undef list

simfile_t* simfile_create();
simfile_t* simfile_load(char* path);

void simfile_export(simfile_t* simfile, char* path);

void simfile_destroy(simfile_t* simfile);

void simfile_add_event(simfile_t* file, const char* event, simfile_event_t value);

#endif//__UTASM_SIMFILE_SIMFILE_H__
