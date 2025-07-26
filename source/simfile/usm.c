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

/* .usm typedefs */

typedef enum
{
	TYPE_NONE,

	TYPE_INTEGER,
	TYPE_FLOAT,

	TYPE_STRING,

	TYPE_NOTES,

	TYPE_NODES
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

		char* str;

		vec_simfile_note_t notes;

		vec_usm_node_ptr_t nodes;
	} value;
	char* name;
} usm_node_t;

typedef struct
{
	uint32_t bpm_offset;
	uint32_t bpm_amount;

	uint32_t scope;

	usm_node_t* global;
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

static usm_node_t* usm_create_node(char* name, usm_node_t* parent)
{
	usm_node_t* node = malloc(sizeof *node);

	node->name = name;
	node->parent = parent;
	node->type = TYPE_NONE;

	return node;
}

static void usm_destroy_node(usm_node_t* node)
{
	// this is probably a hack.
	puts((node->name? node->name : "global"));

	switch (node->type)
	{
		case TYPE_NOTES:
			vector_delete(node->value.notes);
			break;
		case TYPE_NODES:
		{
			vec_usm_node_ptr_t node_vec = node->value.nodes;
			for (int i = 0; i < node_vec.size; i++)
			{
				usm_node_t* tmp;
				vector_pop(node_vec, tmp);

				printf("%s,%d\n", tmp->name, tmp->type);

				usm_destroy_node(tmp);
			}
			vector_delete(node_vec);
		} break;
		case TYPE_STRING:
			free(node->value.str);
			break;
		default:
			break;
	}

	if (node->name) free(node->name);
	free(node);
}

static void usm_print_node(usm_node_t* node, int tab, int pflag)
{
	char tabs[64];
	memset(tabs, ' ', 64);
	tabs[tab] = 0;

	printf("%snode %s:\n", tabs, (node->name? node->name : "global"));

	if (node->parent)
	{
		if (!pflag)
		{
			printf("%s parent:\n", tabs);
			usm_print_node(node->parent, tab+2, 0);
		}
		else printf("%s parent: (hidden);\n", tabs);
	}
	else
	{
		printf("%s parent: (nil);\n", tabs);
	}

	printf("%s --------\n", tabs);
	switch (node->type)
	{
		case TYPE_NONE:
			printf("%s type: none;\n%s value: (nil);\n",tabs,tabs);
			break;
		case TYPE_FLOAT:
			printf("%s type: float;\n%s value: %f;\n",tabs,tabs, node->value.f);
			break;
		case TYPE_INTEGER:
			printf("%s type: integer;\n%s value: %i;\n",tabs,tabs, node->value.i);
			break;
		case TYPE_STRING:
			printf("%s type: string;\n%s value: %s;\n",tabs,tabs, node->value.str);
			break;
		case TYPE_NOTES:
			printf("%s type: notes;\n%s value: (hidden);\n",tabs,tabs);
			break;
		case TYPE_NODES:
			printf("%s type: nodes;\n%s value:\n",tabs,tabs);
			for (int i = 0; i < node->value.nodes.size; i++)
			{
				usm_print_node(node->value.nodes.data[i], tab+2, 1);
			}
			break;
	}
	printf("%s;\n", tabs);
}

/* .usm parser */

#define usm_type_guard(X) if (state->global->type != X && state->global->type) return STATUS_ERROR

static simfile_note_type_t usm_note(char c)
{
	return	(c >= '0' && c <= '7') ||
			c == ',';
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
		//putc(current, stdout);

		if (usm_note(current))
		{
			/*
			 * start figuring out how the notes are snapped by
			 * taking advantage of our knowledge of when a notefield
			 * starts and ends, then diving by overall length and store an
			 * integer offset.
			 */

			/*vec_simfile_note_t notes = state->global.value.notes;
			if (!vector_allocated(notes))
			{
				state->global.type = TYPE_NOTES;
				vector_init(notes);
				vector_new(notes, 4);
			}

			if (current == ',')
			{
				state->bpm_amount = 0;
				state->bpm_offset++;

				continue;
			}*/

			int i = 0;
			while (usm_note(current))
			{
				current = buffer[pos++];

				// ignore this for now
				if (current == ',') continue;

				/*current = buffer[pos++];
				if (current >= '0')
				{
					simfile_note_t note =
					{
						.beat=-1.0f,
						.col=i++,
						.type=current - '0'
					};
					vector_push(notes, note);
				}*/
			}
			state->bpm_amount++;
			continue;
		}

		if (usm_legal(current))
		{
			usm_type_guard(TYPE_NODES);

			if (state->global->type == TYPE_NONE)
			{
				state->global->type = TYPE_NODES;
				vector_init(state->global->value.nodes);
				vector_new(state->global->value.nodes, 4);
			}

			int length = 0;
			while (usm_legal(buffer[pos + length])) length++;

			if (buffer[pos + length] != ':') return 1;
			if (length == 1) return 1;

			// potential bottleneck.
			char* nbuf = malloc(length + 1);
			memcpy(nbuf, buffer + pos, length);
			nbuf[length] = 0;

			puts(nbuf);

			usm_node_t* node = usm_create_node(nbuf, state->global);
			vector_push(state->global->value.nodes, node);
			state->global = node;

			printf("%d\n", ++state->scope);
			pos += length + 1;
			continue;
		}

		if (current == '"')
		{
			usm_type_guard(TYPE_STRING);

			if (state->global->type == TYPE_NONE)
				state->global->type = TYPE_STRING;

			pos++;

			int length = 0;
			while (buffer[pos + length] != '"') length++;

			char* sbuf = malloc(length + 1);
			memcpy(sbuf, buffer + pos, length);
			sbuf[length] = 0;

			puts(sbuf);

			state->global->value.str = sbuf;

			pos += length + 1;

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

	usm_node_t* global = usm_create_node(NULL, NULL);
	state.global = global;

	const char* str = "test";
	char* alloced = malloc(5);
	memcpy(alloced, str, 5);

	usm_node_t* test = usm_create_node(alloced, global);

	test->type = TYPE_INTEGER;
	test->value.i = 0;

	global->type = TYPE_NODES;
	vector_init(global->value.nodes);
	vector_new(global->value.nodes, 1);
	vector_push(global->value.nodes, test);

	puts("hi guys");

	/*char buffer[256];
	while (fgets(buffer, 256, file))
	{
		if (usm_parse(&state, buffer))
		{
			puts("yeah no");
			break;
		}
	}*/

	usm_print_node(global, 0, 0);

	usm_destroy_node(global);

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
	for (int i = 0; i < chart->notes.size; i++)
	{
		simfile_note_t notedata = chart->notes.data[i];
		buffer[chart->key_count] = 0;

		int pos = 0;
		while (pos < chart->key_count)
		{
			buffer[pos] = '0' + ((notedata.col == pos) * notedata.type);
			pos++;
		}

		fprintf(file, "\t\t\t%s\n", buffer);
	}
	fprintf(file, "\t\t;\n\t;\n;");
}
