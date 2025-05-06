#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * List of functions for pipeline:
 * 		unsigned int create_buffer(struct s_graphics_pipeline* self, void* data, unsigned long size, int type);
 * 		int destroy_buffer(struct s_graphics_pipeline* self, unsigned int buffer);
 * ----
 * 		void queue_program(struct s_graphics_pipeline* self, unsigned char size);
 * 		unsigned int create_shader(struct s_graphics_pipeline* self, const char* source, unsigned int type);
 * 		unsigned int finish_program(struct s_graphics_pipeline* self, int flags);
 * 		int destroy_program(struct s_graphics_pipeline* self, unsigned int program);
 * ----
 * 		unsigned int create_texture(struct s_graphics_pipeline* self, unsigned int width, unsigned int height, unsigned char channels, unsigned char* pixels, int flags);
 * 		int destroy_texture(struct s_graphics_pipeline* self, unsigned int texture);
 */

int graphics_create_pipeline(graphics_pipeline_t* pipeline, GladGLContext* context)
{
	if (!vector_create(&pipeline->buffers, sizeof(unsigned int), 4)) return 1;
	if (!vector_create(&pipeline->vertex_arrays, sizeof(unsigned int), 4)) return 1;
	if (!vector_create(&pipeline->texture_cache, sizeof(unsigned int), 4)) return 1;
	if (!vector_create(&pipeline->shader_queue, sizeof(unsigned int), 4)) return 1;

	return 0;
}

void graphics_destroy_pipeline(graphics_pipeline_t pipeline)
{
	GladGLContext* gl = pipeline.context;

	for (int i = 0; i < pipeline.buffers.size; i++)
	{
		unsigned int* buffer = pipeline.buffers.data[i];
		if (!gl->IsBuffer(*buffer)) continue;

		gl->DeleteBuffers(1, buffer);
	}
	for (int i = 0; i < pipeline.vertex_arrays.size; i++)
	{
		unsigned int* vertex_array = pipeline.vertex_arrays.data[i];
		if (!gl->IsVertexArray(*vertex_array)) continue;

		gl->DeleteVertexArrays(1, vertex_array);
	}
}
