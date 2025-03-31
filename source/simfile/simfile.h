#ifndef __UTASM_SIMFILE_SIMFILE_H__
#define __UTASM_SIMFILE_SIMFILE_H__

#define list simfile_list_t

typedef struct
{
	float beat; 
	void value;
} simfile_event_t;

typedef struct
{
	unsigned long size;
	void* items;
} simfile_list_t;

typedef struct
{
	unsigned char size;
	short data;
} simfile_row_t;

typedef struct
{
	char* name;
	char* subtitle;
	char* artist;
	char* author;

	list bpms;
	list stops;

	unsigned char key_count;
	list rows;
} simfile_t;

#undef list

simfile_t* simfile_create();
simfile_t* simfile_load(char* path);

void simfile_add_event(simfile_t* file, const char* event, simfile_event_t value);

#endif//__UTASM_SIMFILE_SIMFILE_H__
