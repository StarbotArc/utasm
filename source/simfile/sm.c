#include "simfile.h"

#include <stdio.h>
#include <stdlib.h>

/* TODO: probably should work on this when we can see notes */
simfile_t* _sm_load(FILE* file) { return NULL; }

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
