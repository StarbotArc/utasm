#ifndef __UTASM_GRAPHICS_GRAPHICS_H__
#define __UTASM_GRAPHICS_GRAPHICS_H__

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

	vector_t buffers;
	vector_t vertex_arrays;

	vector_t texture_cache;

	unsigned int program_queued;
	vector_t shader_queue;

	unsigned int (*create_buffer)(struct s_graphics_pipeline* self, void* data, unsigned long size, int type, int usage);
	int (*destroy_buffer)(struct s_graphics_pipeline* self, unsigned int buffer);

	void (*queue_program)(struct s_graphics_pipeline* self, unsigned char size);
	unsigned int (*create_shader)(struct s_graphics_pipeline* self, const char* source, unsigned int type);
	unsigned int (*finish_program)(struct s_graphics_pipeline* self, int flags);
	int (*destroy_program)(struct s_graphics_pipeline* self, unsigned int program);

	unsigned int (*create_texture)(struct s_graphics_pipeline* self, unsigned int width, unsigned int height, unsigned char channels, unsigned char* pixels, int flags);
	int (*destroy_texture)(struct s_graphics_pipeline* self, unsigned int texture);
} graphics_pipeline_t;

typedef struct s_graphics_drawable
{
	unsigned char header;

	unsigned int ebo;
	unsigned int vbo;

	unsigned int vao;

	void* storage;

	void (*draw)(struct s_graphics_drawable self, struct s_graphics_pipeline pipeline, unsigned int count);
} graphics_drawable_t;

int graphics_create_pipeline(graphics_pipeline_t* pipeline, GladGLContext* context);
void graphics_destroy_pipeline(graphics_pipeline_t pipeline);

#endif//__UTASM_GRAPHICS_GRAPHICS_H__
