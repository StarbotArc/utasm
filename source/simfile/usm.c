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
	fprintf(file, "chart:\n\ttype:\"%s\";\n\tnotedata:\n", "dance-single");

	char buffer[33];
	for (int i = 0; i < simfile->rows.size; i++)
	{
		unsigned int notedata = simfile->rows.items[i].notedata;
		buffer[simfile->key_count] = 0;

		int pos = 0;
		while (pos < simfile->key_count)
		{
			buffer[pos] = '0' + ((notedata >> pos) & 1);
			pos++;
		}

		fprintf(file, "\t\t%s,\n", buffer);
	}
	fprintf(file, "\t;\n;");
}
