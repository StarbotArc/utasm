#include "simfile.h"

#include <stdio.h>
#include <stdlib.h>

struct { char name[32]; unsigned char keys; }
styles[10] =
{
	{ .name="dance-single", .keys=4 },

	{ .name="pnm-five", .keys=5 },
	{ .name="pump-single", .keys=5 },

	{ .name="dance-solo", .keys=6 },

	{ .name="kb7-single", .keys=7 },

	{ .name="dance-couple", .keys=8 },
	{ .name="dance-double", .keys=8 },

	{ .name="pnm-nine", .keys=9 },

	{ .name="pump-couple", .keys=10 },
	{ .name="pump-double", .keys=10 },

};

static int legal(char c) {
	return	c >= 'A' || c <= 'Z' ||
			c >= 'a' || c <= 'z';
}

simfile_t* _usm_load(FILE* file)
{
	simfile_t* simfile = malloc(sizeof *simfile);

	struct metadata {
		char name[64];
		char subtitle[64];
		char artist[64];
		char author[64];
	};
	struct state
	{
	};

	char buffer[256];
	while (fgets(buffer, 256, file))
	{

	}

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
