#ifndef __UTASM_GRAPHICS_GRAPHICS_H__
#define __UTASM_GRAPHICS_GRAPHICS_H__

#include <stdlib.h>

#include <util/vector.h>

#include "glmaid/OpenGL/gl.h"

#define GRAPHICS__BUFFER_TYPE__ELEMENT 1
#define GRAPHICS__BUFFER_TYPE__UNIFORM 2
#define GRAPHICS__BUFFER_TYPE__VERTEX 3

#define GRAPHICS__FINISH_PROGRAM__DESTROY_SHADERS 1

typedef struct s_graphics_pipeline
{
	unsigned int header;
	GladGLContext* context;

	vec_uint32_t buffers;
	vec_uint32_t vertex_arrays;

	vec_uint32_t texture_cache;

	uint32_t program_queued;
	vec_uint32_t shader_queue;

	uint32_t (*create_buffer)(struct s_graphics_pipeline* self, void* data, uint64_t size, uint32_t type, uint32_t usage);
	int (*destroy_buffer)(struct s_graphics_pipeline* self, uint32_t buffer);

	void (*queue_program)(struct s_graphics_pipeline* self, uint8_t size);
	uint32_t (*create_shader)(struct s_graphics_pipeline* self, const char* source, uint32_t type);
	uint32_t (*finish_program)(struct s_graphics_pipeline* self, uint32_t flags);
	int (*destroy_program)(struct s_graphics_pipeline* self, uint32_t program);

	uint32_t (*create_texture)(struct s_graphics_pipeline* self, uint32_t width, uint32_t height, uint8_t channels, uint8_t* pixels, uint32_t flags);
	int (*destroy_texture)(struct s_graphics_pipeline* self, uint32_t texture);
} graphics_pipeline_t;

typedef struct s_graphics_drawable
{
	uint8_t header;

	uint32_t ebo;
	uint32_t vbo;

	uint32_t vao;

	void* storage;

	void (*draw)(struct s_graphics_drawable self, struct s_graphics_pipeline pipeline, uint32_t count);
} graphics_drawable_t;

int graphics_create_pipeline(graphics_pipeline_t* pipeline, GladGLContext* context);
void graphics_destroy_pipeline(graphics_pipeline_t pipeline);

#endif//__UTASM_GRAPHICS_GRAPHICS_H__
