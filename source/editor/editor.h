#ifndef __UTASM_EDITOR_EDITOR_H__
#define __UTASM_EDITOR_EDITOR_H__

#include <graphics/graphics.h>

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

int editor_create(graphics_pipeline_t* pipeline, uint32_t width, uint32_t height);
void editor_destroy(graphics_pipeline_t* pipeline);

void editor_update(double delta);
void editor_draw(graphics_pipeline_t* pipeline);

#endif//__UTASM_EDITOR_EDITOR_H__
