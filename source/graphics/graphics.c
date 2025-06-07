#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * The grand list of pipeline functions.
 */

static unsigned int pipeline_create_buffer(struct s_graphics_pipeline* self, void* data, unsigned long size, int type, int usage);
static int pipeline_destroy_buffer(struct s_graphics_pipeline* self, unsigned int buffer);

static void pipeline_queue_program(struct s_graphics_pipeline* self, unsigned char size);
static unsigned int pipeline_create_shader(struct s_graphics_pipeline* self, const char* source, unsigned int type);
static unsigned int pipeline_finish_program(struct s_graphics_pipeline* self, int flags);
static int pipeline_destroy_program(struct s_graphics_pipeline* self, unsigned int program);

static unsigned int pipeline_create_texture(struct s_graphics_pipeline* self, unsigned int width, unsigned int height, unsigned char channels, unsigned char* pixels, int flags);
static int pipeline_destroy_texture(struct s_graphics_pipeline* self, unsigned int texture);

int graphics_create_pipeline(graphics_pipeline_t* pipeline, GladGLContext* context)
{
	//if (!vector_create(&pipeline->buffers, sizeof(unsigned int), 4)) return 1;
	//if (!vector_create(&pipeline->vertex_arrays, sizeof(unsigned int), 4)) return 1;
	//if (!vector_create(&pipeline->texture_cache, sizeof(unsigned int), 4)) return 1;
	if (!vector_create(&pipeline->shader_queue, sizeof(unsigned int), 4)) return 1;

	pipeline->create_buffer = pipeline_create_buffer;
	pipeline->destroy_buffer = pipeline_destroy_program;

	pipeline->queue_program = pipeline_queue_program;
	pipeline->create_shader = pipeline_create_shader;
	pipeline->finish_program = pipeline_finish_program;
	pipeline->destroy_program = pipeline_destroy_program;

	pipeline->create_texture = pipeline_create_texture;
	pipeline->destroy_texture = pipeline_destroy_texture;

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

unsigned int pipeline_create_buffer(struct s_graphics_pipeline* self, void* data, unsigned long size, int type, int usage)
{
	GladGLContext* gl = self->context;

	unsigned int buffer;
	gl->GenBuffers(1, &buffer);

	gl->BindBuffer(GL_ARRAY_BUFFER, buffer);
	gl->BufferData(GL_ARRAY_BUFFER, size, data, usage);
	gl->BindBuffer(GL_ARRAY_BUFFER, 0);

	return buffer;
}
int pipeline_destroy_buffer(struct s_graphics_pipeline* self, unsigned int buffer)
{
	GladGLContext* gl = self->context;

	gl->DeleteBuffers(1, &buffer);
	return 0;
}

void pipeline_queue_program(struct s_graphics_pipeline* self, unsigned char size)
{
	GladGLContext* gl = self->context;

	self->program_queued = gl->CreateProgram();
}
unsigned int pipeline_create_shader(struct s_graphics_pipeline* self, const char* source, unsigned int type)
{
	GladGLContext* gl = self->context;

	unsigned int* shader = malloc(sizeof *shader);
	shader[0] = gl->CreateShader(type);
	gl->ShaderSource(shader[0], 1, &source, NULL);
	gl->CompileShader(shader[0]);

	char buffer[512];
	int success;

	gl->GetShaderiv(shader[0], GL_COMPILE_STATUS, &success);
	if (success)
	{
		printf("[Shader]: %s\n", buffer);
		return -1;
	}

	vector_add(&self->shader_queue, shader);
	return shader[0];
}
unsigned int pipeline_finish_program(struct s_graphics_pipeline* self, int flags)
{
	GladGLContext* gl = self->context;

	unsigned int program = self->program_queued;

	for (int i = 0; i < self->shader_queue.size; i++)
	{
		unsigned int* shader = self->shader_queue.data[i];
		if (!gl->IsShader(*shader)) continue;

		gl->AttachShader(program, *shader);

		shader[0] = -1;
	}
	gl->LinkProgram(program);

	return program;
}
int pipeline_destroy_program(struct s_graphics_pipeline* self, unsigned int program)
{
	GladGLContext* gl = self->context;

	gl->DeleteProgram(program);
	return 0;
}

unsigned int pipeline_create_texture(struct s_graphics_pipeline* self, unsigned int width, unsigned int height, unsigned char channels, unsigned char* pixels, int flags)
{
	return 0;
}
int pipeline_destroy_texture(struct s_graphics_pipeline* self, unsigned int texture)
{
	return 0;
}
