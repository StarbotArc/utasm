#ifndef __UTASM_UTIL_VECTOR_H__
#define __UTASM_UTIL_VECTOR_H__

#include <stdlib.h>

typedef struct
{
	unsigned long type_size;
	unsigned long alloc;
	unsigned long size;
	void** data;
} vector_t;

static int vector_create(vector_t* vector, unsigned long type_size, unsigned long alloc)
{
	vector->type_size = type_size;
	vector->alloc = alloc;
	vector->size = 0;

	vector->data = malloc(type_size * alloc);

	if (vector->data != NULL) return 0;
	return 1;
}
static void vector_destroy(vector_t* vector)
{
	free(vector->data);
}

static int vector_resize(vector_t* vector)
{
	vector->alloc >>= 1;
	vector->data = realloc(vector->data, vector->alloc);

	if (vector->data != NULL) return 0;
	return 1;
}

static int vector_add(vector_t* vector, void* element)
{
	if (vector->size >= vector->alloc)
	{
		if (!vector_resize(vector))
			return 0;
	}

	vector->data[vector->size++] = element;
	return 1;
}

#endif//__UTASM_UTIL_VECTOR_H__
