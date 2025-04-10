#include "simfile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIMFILE_EXPORT_IF(X, Y) if (!strcmp(X, #Y)) _##Y##_export
#define SIMFILE_EXPORT_TYPE(X) void  _##X##_export(simfile_t* simfile, FILE* file);

SIMFILE_EXPORT_TYPE(ssc)
SIMFILE_EXPORT_TYPE(usm)
SIMFILE_EXPORT_TYPE(sm)

void _sm_export(simfile_t* simfile, FILE* file)
{
	const char* header =
	"#TITLE:%s;\n"
	"#SUBTITLE:%s;\n"
	"#ARTIST:%s;\n"
	"#CREDIT:%s;\n";
	fprintf(file, header, simfile->name, simfile->subtitle, simfile->artist, simfile->author);
	fflush(file);

	puts("sm");
}
void _ssc_export(simfile_t* simfile, FILE* file) {}
void _usm_export(simfile_t* simfile, FILE* file) {}

simfile_t* simfile_create()
{
	simfile_t* simfile = malloc(sizeof *simfile);

	simfile->name = "cheese";
	simfile->subtitle = "mozzerella";
	simfile->artist = "parmesan";
	simfile->author = "romano";

	simfile->key_count = 4;

	simfile->rows.size = 4;
	simfile->rows.items = malloc(sizeof(simfile_item_t) * 4);

	for (int i = 0; i < 4; i++)
	{
		simfile->rows.items[i].notedata = 1 << i;
	}

	return simfile;
}
simfile_t* simfile_load(char* path)
{
	return NULL;
}

void simfile_export(simfile_t* simfile, char* path, char* as)
{
	FILE* file = fopen(path, "w");

	SIMFILE_EXPORT_IF(as, sm)(simfile, file);
	SIMFILE_EXPORT_IF(as, ssc)(simfile, file);
	SIMFILE_EXPORT_IF(as, usm)(simfile, file);

	fclose(file);
}

void simfile_destroy(simfile_t* simfile)
{
	free(simfile->rows.items);
	free(simfile);
}

void simfile_add_event(simfile_t* file, const char* event, simfile_event_t value)
{
}

