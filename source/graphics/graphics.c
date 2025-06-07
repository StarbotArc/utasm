#include "graphics.h"

#include <stdio.h>
#include <stdint.h>

/*
 * The grand list of pipeline functions.
 */

static uint32_t pipeline_create_buffer(struct s_graphics_pipeline* self, void* data, uint64_t size, uint32_t type, uint32_t usage);
static int pipeline_destroy_buffer(struct s_graphics_pipeline* self, uint32_t buffer);

static void pipeline_queue_program(struct s_graphics_pipeline* self, uint8_t size);
static uint32_t pipeline_create_shader(struct s_graphics_pipeline* self, const char* source, uint32_t type);
static uint32_t pipeline_finish_program(struct s_graphics_pipeline* self, uint32_t flags);
static int pipeline_destroy_program(struct s_graphics_pipeline* self, uint32_t program);

static uint32_t pipeline_create_texture(struct s_graphics_pipeline* self, uint32_t width, uint32_t height, uint8_t channels, uint8_t* pixels, uint32_t flags);
static int pipeline_destroy_texture(struct s_graphics_pipeline* self, uint32_t texture);

int graphics_create_pipeline(graphics_pipeline_t* pipeline, GladGLContext* context)
{
	//if (vector_create(&pipeline->buffers, sizeof(uint32_t), 4)) return 1;
	//if (vector_create(&pipeline->vertex_arrays, sizeof(uint32_t), 4)) return 1;
	// //if (vector_create(&pipeline->texture_cache, sizeof(uint32_t), 4)) return 1;
	//if (vector_create(&pipeline->shader_queue, sizeof(uint32_t), 4)) return 1;

	pipeline->create_buffer = pipeline_create_buffer;
	pipeline->destroy_buffer = pipeline_destroy_buffer;

	pipeline->queue_program = pipeline_queue_program;
	pipeline->create_shader = pipeline_create_shader;
	pipeline->finish_program = pipeline_finish_program;
	pipeline->destroy_program = pipeline_destroy_program;

	pipeline->create_texture = pipeline_create_texture;
	pipeline->destroy_texture = pipeline_destroy_texture;

	pipeline->context = context;

	return 0;
}

void graphics_destroy_pipeline(graphics_pipeline_t pipeline)
{
	GladGLContext* gl = pipeline.context;

	for (int i = 0; i < pipeline.buffers.size; i++)
	{
		uint32_t buffer = pipeline.buffers.data[i];
		if (!gl->IsBuffer(buffer)) continue;

		gl->DeleteBuffers(1, &buffer);
	}
	for (int i = 0; i < pipeline.vertex_arrays.size; i++)
	{
		uint32_t vertex_array = pipeline.vertex_arrays.data[i];
		if (!gl->IsVertexArray(vertex_array)) continue;

		gl->DeleteVertexArrays(1, &vertex_array);
	}
}

uint32_t pipeline_create_buffer(struct s_graphics_pipeline* self, void* data, uint64_t size, uint32_t type, uint32_t usage)
{
	GladGLContext* gl = self->context;

	unsigned int buffer;
	gl->GenBuffers(1, &buffer);

	gl->BindBuffer(GL_ARRAY_BUFFER, buffer);
	gl->BufferData(GL_ARRAY_BUFFER, size, data, usage);
	gl->BindBuffer(GL_ARRAY_BUFFER, 0);

	return buffer;
}
int pipeline_destroy_buffer(struct s_graphics_pipeline* self, uint32_t buffer)
{
	GladGLContext* gl = self->context;

	gl->DeleteBuffers(1, &buffer);
	return 0;
}

void pipeline_queue_program(struct s_graphics_pipeline* self, uint8_t size)
{
	GladGLContext* gl = self->context;

	self->program_queued = gl->CreateProgram();
}
unsigned int pipeline_create_shader(struct s_graphics_pipeline* self, const char* source, uint32_t type)
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

	//vector_add(&self->shader_queue, shader);
	return shader[0];
}
unsigned int pipeline_finish_program(struct s_graphics_pipeline* self, uint32_t flags)
{
	GladGLContext* gl = self->context;

	uint32_t program = self->program_queued;

	for (int i = 0; i < self->shader_queue.size; i++)
	{
		uint32_t shader = self->shader_queue.data[i];
		if (!gl->IsShader(shader)) continue;

		gl->AttachShader(program, shader);

		self->shader_queue.data[i] = -1;
	}
	gl->LinkProgram(program);

	return program;
}
int pipeline_destroy_program(struct s_graphics_pipeline* self, uint32_t program)
{
	GladGLContext* gl = self->context;

	gl->DeleteProgram(program);
	return 0;
}

uint32_t pipeline_create_texture(struct s_graphics_pipeline* self, uint32_t width, uint32_t height, uint8_t channels, uint8_t* pixels, uint32_t flags)
{
	return 0;
}
int pipeline_destroy_texture(struct s_graphics_pipeline* self, uint32_t texture)
{
	return 0;
}
