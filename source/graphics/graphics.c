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
	vector_init(pipeline->buffers);
	vector_init(pipeline->vertex_arrays);
	vector_init(pipeline->texture_cache);
	vector_init(pipeline->shader_queue);

	vector_new(pipeline->buffers, 4);
	vector_new(pipeline->vertex_arrays, 4);
	vector_new(pipeline->texture_cache, 4);
	vector_new(pipeline->shader_queue, 4);

	if (!vector_allocated(pipeline->buffers)) return 1;
	if (!vector_allocated(pipeline->vertex_arrays)) return 1;
	if (!vector_allocated(pipeline->texture_cache)) return 1;
	if (!vector_allocated(pipeline->shader_queue)) return 1;

	pipeline->create_buffer = pipeline_create_buffer;
	pipeline->destroy_buffer = pipeline_destroy_buffer;

	pipeline->queue_program = pipeline_queue_program;
	pipeline->create_shader = pipeline_create_shader;
	pipeline->finish_program = pipeline_finish_program;
	pipeline->destroy_program = pipeline_destroy_program;

	pipeline->create_texture = pipeline_create_texture;
	pipeline->destroy_texture = pipeline_destroy_texture;

	pipeline->context = context;

	pipeline->program_queued = -1;

	return 0;
}

void graphics_destroy_pipeline(graphics_pipeline_t pipeline)
{
	GladGLContext* gl = pipeline.context;

	if (vector_allocated(pipeline.buffers))
	{
		for (int i = 0; i < pipeline.buffers.size; i++)
		{
			uint32_t buffer = pipeline.buffers.data[i];
			if (!gl->IsBuffer(buffer)) continue;

			gl->DeleteBuffers(1, &buffer);
		}
	}
	if (vector_allocated(pipeline.vertex_arrays))
	{
		for (int i = 0; i < pipeline.vertex_arrays.size; i++)
		{
			uint32_t vertex_array = pipeline.vertex_arrays.data[i];
			if (!gl->IsVertexArray(vertex_array)) continue;

			gl->DeleteVertexArrays(1, &vertex_array);
		}
	}

	vector_delete(pipeline.buffers);
	vector_delete(pipeline.vertex_arrays);
	vector_delete(pipeline.texture_cache);
	vector_delete(pipeline.shader_queue);
	free(gl);
}

uint32_t pipeline_create_buffer(struct s_graphics_pipeline* self, void* data, uint64_t size, uint32_t type, uint32_t usage)
{
	GladGLContext* gl = self->context;

	unsigned int buffer;
	gl->GenBuffers(1, &buffer);

	gl->BindBuffer(type, buffer);
	gl->BufferData(type, size, data, usage);

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
	if (self->program_queued != -1) return;

	GladGLContext* gl = self->context;

	self->program_queued = gl->CreateProgram();
}
unsigned int pipeline_create_shader(struct s_graphics_pipeline* self, const char* source, uint32_t type)
{
	GladGLContext* gl = self->context;

	unsigned int shader;
	shader = gl->CreateShader(type);
	gl->ShaderSource(shader, 1, &source, NULL);
	gl->CompileShader(shader);

	char buffer[512];
	int success;

	gl->GetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		gl->GetShaderInfoLog(shader, 512, NULL, buffer);
		printf("[Shader]: %s\n", buffer);
		return -1;
	}

	vector_push(self->shader_queue, shader);
	return shader;
}
unsigned int pipeline_finish_program(struct s_graphics_pipeline* self, uint32_t flags)
{
	GladGLContext* gl = self->context;

	uint32_t program = self->program_queued;

	for (int i = 0; i < self->shader_queue.size; i++)
	{
		uint32_t shader = self->shader_queue.data[i];

		if (!gl->IsShader(shader)) continue;
		printf("Passed Shader: %d\n", shader);

		gl->AttachShader(program, shader);

		self->shader_queue.data[i] = -1;
	}
	gl->LinkProgram(program);

	char buffer[512];
	int success;

	gl->GetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		gl->GetProgramInfoLog(program, 512, NULL, buffer);
		printf("[Program]: %s\n", buffer);
		return -1;
	}

	self->program_queued = -1;

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
