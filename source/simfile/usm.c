#include "simfile.h"

#include <stdio.h>
#include <stdlib.h>

typedef enum
{
	TYPE_NONE,

	TYPE_INTEGER,
	TYPE_FLOAT,

	TYPE_STRING,

	TYPE_NODE
} _umeta_type;

typedef struct struct_umeta_node {
	_umeta_type type;
	union {
		int i;
		float f;
		char* s;
		struct struct_umeta_node* node;
	} value;
	char* name;
} _umeta_node;

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
char difficulties[][8] =
{
	"bg",
	"ez",
	"nm",
	"hd",
	"in",
	"ed"
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

	simfile_chart_t* chart = simfile->charts;

	char buffer[33];
	for (int i = 0; i < chart->rows.size; i++)
	{
		uint32_t notedata = chart->rows.data[i];
		buffer[chart->key_count] = 0;

		int pos = 0;
		while (pos < chart->key_count)
		{
			buffer[pos] = '0' + ((notedata >> pos) & 1);
			pos++;
		}

		fprintf(file, "\t\t%s,\n", buffer);
	}
	fprintf(file, "\t;\n;");
}
