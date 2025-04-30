#ifndef __UTASM_GRAPHICS_GRAPHICS_H__
#define __UTASM_GRAPHICS_GRAPHICS_H__

#include "../graphics/glmaid/OpenGL/gl.h"

#define GRAPHICS__FINISH_PROGRAM__DESTROY_SHADERS 1

typedef struct s_graphics_pipeline
{
	GladGLContext* context;

	unsigned int* texture_cache;

	unsigned int program_queued;
	unsigned int* shader_queue;

	void (*queue_program)(struct s_graphics_pipeline* self, unsigned char size);
	unsigned int (*create_shader)(struct s_graphics_pipeline* self, const char* source, unsigned int type);
	unsigned int (*finish_program)(struct s_graphics_pipeline* self, int flags);

	unsigned int (*create_texture)(struct s_graphics_pipeline* self, unsigned int width, unsigned int height, unsigned char channels, unsigned char pixels, int flags);
	int (*destroy_texture)(struct s_graphics_pipeline* self, unsigned int texture);
} graphics_pipeline_t;

int graphics_create_pipeline(graphics_pipeline_t* pipeline, GladGLContext* context);
void graphics_destroy_pipeline(graphics_pipeline_t pipeline);

#endif//__UTASM_GRAPHICS_GRAPHICS_H__
