#include "editor.h"

uint32_t quad_ebo;
uint32_t quad_vao;
uint32_t quad_vbo;

uint32_t shader;

int editor_create(graphics_pipeline_t* pipeline, uint32_t width, uint32_t height)
{
	unsigned int q_ind[6] = {
		0, 1, 3,
		3, 1, 2
	};

	float quad[8] = {
		0.5f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f,
		-0.5f, 0.5f,
	};

	GladGLContext* gl = pipeline->context;

	gl->GenVertexArrays(1, &quad_vao);
	gl->BindVertexArray(quad_vao);

	quad_ebo = pipeline->create_buffer(pipeline, q_ind, sizeof(q_ind), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
	quad_vbo = pipeline->create_buffer(pipeline, quad, sizeof(quad), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

	gl->VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	gl->EnableVertexAttribArray(0);

	gl->BindBuffer(GL_ARRAY_BUFFER, 0);
	gl->BindVertexArray(0);

	gl->DisableVertexAttribArray(0);

	pipeline->queue_program(pipeline, 2);
	pipeline->create_shader(pipeline, vertex_shader_source, GL_VERTEX_SHADER);
	pipeline->create_shader(pipeline, fragment_shader_source, GL_FRAGMENT_SHADER);
	shader = pipeline->finish_program(pipeline, 0);

	printf("quad_ebo: %d\nquad_vao: %d\nquad_vbo: %d\n", quad_ebo, quad_vao, quad_vbo);

	return 0;
}
void editor_destroy(graphics_pipeline_t* pipeline)
{
	pipeline->context->DeleteVertexArrays(1, &quad_vao);

	pipeline->destroy_program(pipeline, shader);

	pipeline->destroy_buffer(pipeline, quad_ebo);
	pipeline->destroy_buffer(pipeline, quad_vbo);
}

void editor_update(double delta)
{
}
void editor_draw(graphics_pipeline_t* pipeline)
{
	GladGLContext* gl = pipeline->context;

	gl->EnableVertexAttribArray(0);

	gl->BindVertexArray(quad_vao);

	gl->UseProgram(shader);

	gl->DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
