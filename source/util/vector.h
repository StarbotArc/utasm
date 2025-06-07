#ifndef __UTASM_UTIL_VECTOR_H__
#define __UTASM_UTIL_VECTOR_H__

#include <stdlib.h>
#include <stdint.h>

/* deprecated immediately

typedef struct
{
	unsigned long type_size;
	unsigned long alloc;
	unsigned long size;
	void** data;
} vector_t;

inline int vector_create(vector_t* vector, unsigned long type_size, unsigned long alloc)
{
	vector->type_size = type_size;
	vector->alloc = alloc;
	vector->size = 0;

	vector->data = malloc(type_size * alloc);

	if (vector->data == NULL) return 1;
	return 0;
}
inline void vector_destroy(vector_t* vector)
{
	free(vector->data);
}

inline int vector_resize(vector_t* vector)
{
	vector->alloc >>= 1;
	vector->data = realloc(vector->data, vector->alloc);

	if (vector->data == NULL) return 1;
	return 0;
}

inline int vector_add(vector_t* vector, void* element)
{
	if (vector->size >= vector->alloc)
	{
		if (!vector_resize(vector))
			return 0;
	}

	vector->data[vector->size++] = element;
	return 1;
} */

// smart edition:

#define vector(X)	struct /* smart_vector_##X */ { \
						uint64_t alloc; \
						uint64_t size;  \
						X* data; \
					}
#define vector_new(variable, alloced)	variable->t_size = sizeof(type); \
										variable->alloc = alloced; \
										variable->size = 0; \
										variable->data = malloc(sizeof *variable->data * variable->alloc)
#define vector_allocated(variable)	variable->data == NULL
#define vector_delete(variable)	free(variable->data)

#endif//__UTASM_UTIL_VECTOR_H__
