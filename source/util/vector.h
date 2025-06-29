#ifndef __UTASM_UTIL_VECTOR_H__
#define __UTASM_UTIL_VECTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/*
 * This is a stupidly complex, macro system that aims to create vectors.
 */

/*
 * Defines a vector structure by type X.
 */
#define vector(X)	struct /* smart_vector_##X */ { \
						uint64_t alloc; \
						uint64_t size;  \
						X* data; \
					}

/*
 * Initializes vectors, so they don't break in allocation
 */
#define vector_init(variable)	variable.alloc = 0;

/*
 * Creates vector and allocates memory needed for it.
 * ASSERTS: alloced > 0
 * ASSERTS: variable.alloc == 0
 * ASSERTS: variable.data != NULL
 */
#define vector_new(variable,alloced)	do { \
											assert(("Allocation size is zero.", (uint64_t) alloced)); \
											assert(("Variable is already allocated.", variable.alloc == 0)); \
											variable.alloc = (uint64_t) alloced; \
											variable.size = 0; \
											variable.data = malloc(sizeof *variable.data * variable.alloc); \
											assert(("Allocation to variable's data is null.", variable.data != NULL)); \
										} while (0)

/*
 * Resizes vector memory in the heap.
 * ASSERTS: variable.data (re-allocated) != NULL
 */
#define vector_resize(variable)	do { \
									variable.alloc <<= 1; \
									variable.data = realloc(variable.data, sizeof *variable.data * variable.alloc); \
									assert(("Reallocation to variable's data is null.", variable.data != NULL)); \
								} while (0)

/*
 * Pushes an element to vector, and resizes if vector gets too full.
 * ASSERTS: [read vector_allocated]
 * ASSERTS: [read vector_resize]
 */
#define vector_push(variable, element)	do { \
											assert(("Pushing to an uninitialized vector.", variable.alloc > 0 && variable.data)); \
											if (variable.size >= variable.alloc) \
											{ \
												vector_resize(variable); \
											} \
											variable.data[variable.size++] = element; \
										} while (0)

/*
 * Pops element from vector and moves it to a variable.
 * ASSERTS: from.size > 0
 * ASSERTS: [read vector_allocated]
 */
#define vector_pop(from, to)	do { \
									assert(("Popping an element from a vector with no size.", from.size)); \
									assert(("Popping an element from an uninitialized vector.", from.alloc > 0 && from.data)); \
									to = from.data[from.size--]; \
								} while (0)

/*
 * Checks if a vector has been allocated.
 */
#define vector_allocated(variable)	(variable.alloc > 0 && variable.data)

/*
 * Frees the vector.
 * ASSERTS: variable.data != NULL
 */
#define vector_delete(variable)	do { \
									assert(("Variable is not allocated", variable.data != NULL)); \
									free(variable.data); \
									variable.alloc = 0; \
									variable.size = 0; \
								} while (0)

#endif//__UTASM_UTIL_VECTOR_H__
