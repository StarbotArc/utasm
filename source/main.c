#include <stdio.h>
#include <stdlib.h>

#include "window/window.h"

extern unsigned char icon_raw[];

static const char* vertex_shader_source =
"#version 440 core\n"
"layout (location=0) in vec4 position;\n"
"void main()\n {"
"	gl_Position = position;"
"}\n"
;

static const char* fragment_shader_source =
"#version 440 core\n"
"out vec4 FragColor;\n"
"void main()\n {"
"	FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
"}\n"
;

struct
{
	union
	{
		unsigned int vao;
		unsigned int vbo;
	} line_model;
	union
	{
		unsigned int generic;
	} shaders;
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

		return 0;
	}

	return shader;
}
static unsigned int create_program(GladGLContext* gl, unsigned int n, unsigned int* shaders)
{
	unsigned int program = gl->CreateProgram();

	for (int i = 0; i < n; i++)
	{
		gl->AttachShader(program, shaders[i]);
	}
	gl->LinkProgram(program);

	int success;
	char info[512];
	gl->GetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		gl->GetProgramInfoLog(program, 512, NULL, info);
		printf("Program error: %s\n", info);

		return 0;
	}

	return program;
}

static WindowAPICreate create(window_context_t window)
{
	GladGLContext* gl = window.context;
	gl->Viewport(0, 0, 900, 900);

	gl->GenVertexArrays(1, &enviroment.line_model.vao);
	gl->GenBuffers(1, &enviroment.line_model.vbo);

	gl->BindVertexArray(enviroment.line_model.vao);

	float line[4] = { -0.5f, 0.0f, 0.5f, 0.0f };

	gl->BindBuffer(GL_ARRAY_BUFFER, enviroment.line_model.vbo);
	gl->BufferData(GL_ARRAY_BUFFER, 4 * sizeof *line, line, GL_STATIC_DRAW);

	gl->VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	gl->BindBuffer(GL_ARRAY_BUFFER, 0);
	gl->BindVertexArray(0);

	unsigned int shader[] = {
		create_shader(gl, vertex_shader_source, GL_VERTEX_SHADER),
		create_shader(gl, fragment_shader_source, GL_FRAGMENT_SHADER),
	};

	enviroment.shaders.generic = create_program(gl, 2, shader);

	gl->ClearColor(0.0625f, 0.075f, 0.1625f, 1.0f);
	return 0;
}
static WindowAPILoop loop(window_context_t window)
{
	window.context->Clear(GL_COLOR_BUFFER_BIT);

	window.context->UseProgram(enviroment.shaders.generic);
	window.context->BindVertexArray(enviroment.line_model.vao);

	window.context->EnableVertexAttribArray(0);
	window.context->DrawArrays(GL_LINES, 0, 2);
	window.context->DisableVertexAttribArray(0);

	window.context->BindVertexArray(0);

	window.context->UseProgram(enviroment.shaders.generic);
}
static WindowAPIDestroy destroy(window_context_t window)
{
}

static WindowAPIResize resize(window_context_t window, int width, int height)
{
	printf("Resized: %d, %d\n", width, height);
	window.context->Viewport(0, 0, width, height);
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

	if (window_library_init()) return 1;

	window_t* window = window_create("UtaSM", 900, 900);
	window_icon_t icon = { .width=128, .height=128, .data=icon_raw };

	window_set_icon(window, &icon);

	window_set_create_callback(window, create);
	window_set_loop_callback(window, loop);
	window_set_destroy_callback(window, destroy);

	window_set_resize_callback(window, resize);

	window_run(window);

	window_destroy(window);

	window_library_destroy();

	return 0;
}
