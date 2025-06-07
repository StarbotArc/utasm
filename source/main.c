#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "window/window.h"
#include "simfile/simfile.h"

#include "scene/scene.h"

#define F(X) (int)X+289

extern uint8_t icon_raw[];

struct
{
	double last_clock;

	scene_t scene;
} enviroment;

static WindowAPICreate create(window_context_t window)
{
	GladGLContext* gl = window.context.context;
	printf("GL: %p\n", gl);

	gl->Viewport(0, 0, window.width, window.height);
	gl->ClearColor(0.0625f, 0.075f, 0.1625f, 1.0f);

	return 0;
}
static WindowAPILoop loop(window_context_t window)
{
	double now = window_clock() - enviroment.last_clock;
	scene_update(enviroment.scene, now);

	enviroment.last_clock = now;
}
static WindowAPIDestroy destroy(window_context_t window)
{
}

static WindowAPIResize resize(window_context_t window, uint32_t width, uint32_t height)
{
	GladGLContext* gl = window.context.context;

	printf("Resized: %d, %d\n", width, height);
	gl->Viewport(0, 0, width, height);
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
*/
static WindowAPIKeyPress key_press(int button, int actions, int mods)
{
	switch (button)
	{
		case F(12):
			break;
	}
}
/* static WindowAPIKeyType key_type(int code)
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

	window_set_key_press_callback(window_1, key_press);

	window_run(window_1);

	window_destroy(window_1);

	window_library_destroy();

	return 0;
}
