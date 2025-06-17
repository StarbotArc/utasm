#include "simfile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIMFILE_LOAD_IF(X, Y) if (!strcmp(X, #Y)) return _##Y##_load
#define SIMFILE_LOAD_TYPE(X) extern simfile_t* _##X##_load(FILE* file);

#define SIMFILE_EXPORT_IF(X, Y) if (!strcmp(X, #Y)) _##Y##_export
#define SIMFILE_EXPORT_TYPE(X) extern void  _##X##_export(simfile_t* simfile, FILE* file);

SIMFILE_LOAD_TYPE(usm)

SIMFILE_EXPORT_TYPE(sm)
SIMFILE_EXPORT_TYPE(ssc)
SIMFILE_EXPORT_TYPE(usm)

simfile_t* simfile_create()
{
	simfile_t* simfile = malloc(sizeof *simfile);

	simfile->name = "cheese";
	simfile->subtitle = "mozzerella";
	simfile->artist = "parmesan";
	simfile->author = "romano";

	simfile->charts = malloc(sizeof *simfile->charts);
	simfile_chart_t* chart = simfile->charts;

	vector_new(chart->rows, 4);

	for (int i = 0; i < 4; i++)
	{
		chart->rows.data[i] = 1 << i;
		printf("%d\n", chart->rows.data[i]);
	}

	return simfile;
}
simfile_t* simfile_load(char* path)
{
	FILE* file = fopen(path, "r");

	const char* as = path;

	SIMFILE_LOAD_IF(as - 3, usm)(file);
	return NULL;
}

void simfile_export(simfile_t* simfile, char* path)
{
	FILE* file = fopen(path, "w");

	const char* as = path + strlen(path);
	for (int i = strlen(path); *(as - 1) != '.'; i--) as--;

	SIMFILE_EXPORT_IF(as, sm)(simfile, file);
	SIMFILE_EXPORT_IF(as, ssc)(simfile, file);
	SIMFILE_EXPORT_IF(as, usm)(simfile, file);

	fclose(file);
}

void simfile_destroy(simfile_t* simfile)
{
	//free(simfile->rows.items);
	free(simfile);
}

void simfile_add_event(simfile_chart_t* file, simfile_event_type_t type, simfile_event_generic_t event)
{

}

