#include "simfile.h"

#include <stdio.h>
#include <stdlib.h>

#include <util/bitfield.h>

/* .usm internal constants. */

struct usm_style { const char* name; unsigned char keys; }
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
#define USM_STYLE_AMOUNT 10

const char* difficulties[6] =
{
	"beginner",
	"easy",
	"normal",
	"hard",
	"insane",
	"edit"
};

typedef enum
{
	TYPE_NONE,

	TYPE_INTEGER,
	TYPE_FLOAT,

	TYPE_STRING,

	TYPE_NOTES,

	TYPE_NODE
} usm_type_t;

typedef enum
{
	STATUS_SUCCESS,

	// TODO: get more specific once finished.
	STATUS_ERROR,
} usm_status_t;

typedef vector(struct struct_usm_node*) vec_usm_node_ptr_t;

typedef struct struct_usm_node
{
	struct struct_usm_node* parent;

	usm_type_t type;
	union {
		int i;
		float f;
		const char* str;
		bitfield_t bitfield;
		vec_usm_node_ptr_t nodes;
	} value;
	char* name;
} usm_node_t;

typedef struct
{
	uint32_t scope;
	usm_node_t global;
} usm_state_t;

/* .usm utilities */

static const char* usm_find_type(simfile_style_t style, uint8_t key_count)
{
	const char* begins = style_to_string(style);
	uint32_t length = strlen(begins) - 1;

	for (int i = 0; i < USM_STYLE_AMOUNT; i++)
	{
		struct usm_style selected = styles[i];

		if (selected.keys == key_count)
		{
			if (!strncmp(begins, selected.name, length)) return selected.name;
		}
	}

	return "unknown";
}

/* .usm parser */

static int usm_note(char c)
{

}

static int usm_legal(char c) {
	return	(c >= 'A' && c <= 'Z') ||
			(c >= 'a' && c <= 'z') ||
			(c >= '0' && c <= '9');
}

static usm_status_t usm_parse(usm_state_t* state, char* buffer)
{
	int pos = 0;
	while (buffer[pos] != '\0')
	{
		char current = buffer[pos];
		putc(current, stdout);

		if (usm_note(current))
		{

		}

		if (usm_legal(current))
		{
			int length = 0;
			while (usm_legal(buffer[pos + length])) length++;

			if (buffer[pos + length] != ':') return 1;
			if (length == 1) return 1;

			// potential bottleneck.
			char* nbuf = malloc(length + 1);
			memcpy(nbuf, buffer + pos, length);
			nbuf[length] = 0;

			puts(nbuf);

			free(nbuf);

			state->scope++;
			pos += length + 1;
			continue;
		}

		if (current == '"')
		{
			pos++;

			int length = 0;
			while (1)
			{
				if (buffer[pos + length] == '"')
				{
					char* nbuf = malloc(length + 1);
					memcpy(nbuf, buffer + pos, length);
					nbuf[length] = 0;

					puts(nbuf);

					free(nbuf);

					pos += length + 1;
					break;
				}

				length++;
			}
			continue;
		}

		if (current == ';')
		{
			state->scope--;
		}

		pos++;
	}

	return 0;
}

/* .usm simfile implementations. */

simfile_t* _usm_load(FILE* file)
{
	// WARNING: don't malloc here lol
	// simfile_t* simfile = malloc(sizeof *simfile);

	struct metadata {
		char name[64];
		char subtitle[64];
		char artist[64];
		char author[64];
	};

	usm_state_t state;
	state.scope = 0;

	usm_node_t global = state.global;

	global.parent = NULL;
	global.name = "global";

	puts("hi guys");

	char buffer[256];
	while (fgets(buffer, 256, file))
	{
		if (usm_parse(&state, buffer))
		{
			puts("yeah no");
			break;
		}
	}

	return NULL;
}

void _usm_export(simfile_t* simfile, FILE* file)
{
	const char* header_temp =
	"metadata:\n"
	"	name:\"%s\";\n"
	"	subtitle:\"%s\";\n"
	"	artist:\"%s\";\n"
	"	author:\"%s\";\n"
	";\n";
	fprintf(
			file, header_temp,
			simfile->name,
			simfile->subtitle,
			simfile->artist,
			simfile->author
		);

	simfile_chart_t* chart = simfile->charts;
	const char* chart_temp =
	"chart:\n"
	"	%s%d:\n"
	"		type:\"%s\";\n"
	"		notedata:\n";
	fprintf(
		file, chart_temp,
		difficulties[chart->difficulty],
		chart->meter,
		usm_find_type(chart->style, chart->key_count)
	);

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

		fprintf(file, "\t\t\t%s,\n", buffer);
	}
	fprintf(file, "\t\t;\n\t;\n;");
}
