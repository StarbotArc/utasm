#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "window/window.h"
#include "simfile/simfile.h"

extern unsigned char icon_raw[];

static const char* vertex_shader_source =
"#version 330 core\n"
"layout (location=0) in vec4 position;\n"
"uniform vec2 dimensions;\n"
"uniform float scroll;\n"
"void main()\n {"
"	float shift = (gl_InstanceID - mod(scroll, 1.0f)) * 512.0f;\n"
"	gl_Position = (position + vec4(0.0f, 256.0f - shift + dimensions.y, 0.0f, 0.0f)) / vec4(dimensions, 1.0f, 1.0f);\n"
"}\n"
;

static const char* fragment_shader_source =
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color_it_up;\n"
"void main()\n {"
"	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n"
;

struct
{
	struct
	{
		unsigned int vao;
		unsigned int vbo;
	} line_model;
	struct
	{
		struct
		{
			unsigned int id;

			unsigned int u_dimensions;
			unsigned int u_scroll;
		} generic;
	} shaders;

	float time;
} enviroment;

static unsigned int create_shader(GladGLContext* gl, const char* source, unsigned int type)
{
	unsigned int shader = gl->CreateShader(type);
	gl->ShaderSource(shader, 1, &source, NULL);
	gl->CompileShader(shader);

	int success;
	char info[512];
	gl->GetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		gl->GetShaderInfoLog(shader, 512, NULL, info);
		printf("Shader error: %s\n", info);

		return -1;
	}

	return shader;
}
static unsigned int create_program(GladGLContext* gl, int n, unsigned int* shaders)
{
	unsigned int program = gl->CreateProgram();

	for (int i = 0; i < n; i++)
	{
		gl->AttachShader(program, shaders[i]);
	}
	gl->LinkProgram(program);

	for (int i = 0; i < n; i++)
	{
		gl->DeleteShader(shaders[i]);
	}

	int success;
	char info[512];
	gl->GetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		gl->GetProgramInfoLog(program, 512, NULL, info);
		printf("Program error: %s\n", info);

		return -1;
	}

	return program;
}

static WindowAPICreate create(window_context_t window)
{
	GladGLContext* gl = window.context;
	gl->Viewport(0, 0, window.width, window.height);

	gl->GenVertexArrays(1, &enviroment.line_model.vao);
	gl->GenBuffers(1, &enviroment.line_model.vbo);

	gl->BindVertexArray(enviroment.line_model.vao);

	float line[4] = { -256.0f, 0.0f, 256.0f, 0.0f };

	gl->BindBuffer(GL_ARRAY_BUFFER, enviroment.line_model.vbo);
	gl->BufferData(GL_ARRAY_BUFFER, 4 * sizeof *line, line, GL_STATIC_DRAW);

	gl->VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	gl->BindBuffer(GL_ARRAY_BUFFER, 0);
	gl->BindVertexArray(0);

	unsigned int shader[] = {
		create_shader(gl, vertex_shader_source, GL_VERTEX_SHADER),
		create_shader(gl, fragment_shader_source, GL_FRAGMENT_SHADER),
	};

	enviroment.shaders.generic.id = create_program(gl, 2, shader);
	enviroment.shaders.generic.u_scroll = gl->GetUniformLocation(enviroment.shaders.generic.id, "scroll");
	enviroment.shaders.generic.u_dimensions = gl->GetUniformLocation(enviroment.shaders.generic.id, "dimensions");

	gl->UseProgram(enviroment.shaders.generic.id);
	gl->Uniform2f(enviroment.shaders.generic.u_dimensions, window.width, window.height);

	gl->ClearColor(0.0625f, 0.075f, 0.1625f, 1.0f);

	return 0;
}
static WindowAPILoop loop(window_context_t window)
{
	GladGLContext* gl = window.context;

	enviroment.time += 0.02f;

	gl->Clear(GL_COLOR_BUFFER_BIT);

	gl->UseProgram(enviroment.shaders.generic.id);
	gl->Uniform1f(enviroment.shaders.generic.u_scroll, enviroment.time);

	gl->BindVertexArray(enviroment.line_model.vao);

	gl->EnableVertexAttribArray(0);
	gl->DrawArraysInstanced(GL_LINES, 0, 2, 2 + (window.height >> 8));
}
static WindowAPIDestroy destroy(window_context_t window)
{
}

static WindowAPIResize resize(window_context_t window, int width, int height)
{
	printf("Resized: %d, %d\n", width, height);
	window.context->Viewport(0, 0, width, height);

	window.context->UseProgram(enviroment.shaders.generic.id);
	window.context->Uniform2f(enviroment.shaders.generic.u_dimensions, width, height);
}

/* static WindowAPIMouseMove mouse_move(double x, double y)
{
}
static WindowAPIMousePress mouse_press(int button, int actions, int mods)
{
}
static WindowAPIMouseScroll mouse_scroll(double x, double y)
{
}

static WindowAPIKeyPress key_press(int button, int actions, int mods)
{
}
static WindowAPIKeyType key_type(int code)
{
} */

int main()
{
	puts("Hello world!");

	simfile_t* simfile = simfile_create();
	simfile_export(simfile, "file.usm");
	simfile_destroy(simfile);

	if (window_library_init()) return 1;

	window_t* window_1 = window_create("UtaSM", 900, 900);
	window_icon_t icon = { .width=128, .height=128, .data=icon_raw };

	window_set_icon(window_1, &icon);

	window_set_create_callback(window_1, create);
	window_set_loop_callback(window_1, loop);
	window_set_destroy_callback(window_1, destroy);

	window_set_resize_callback(window_1, resize);

	window_run(window_1);

	window_destroy(window_1);

	window_library_destroy();

	return 0;
}
