#include "simfile.h"

#include <stdio.h>
#include <stdlib.h>

simfile_t* _usm_load(FILE* file)
{
	return NULL;
}

void _usm_export(simfile_t* simfile, FILE* file)
{
	const char* header =
	"metadata:\n"
	"	name:\"%s\";\n"
	"	subtitle:\"%s\";\n"
	"	artist:\"%s\";\n"
	"	author:\"%s\";\n"
	";\n";
	fprintf(file, header, simfile->name, simfile->subtitle, simfile->artist, simfile->author);
}
