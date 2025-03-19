#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

#include "window/window.h"

extern unsigned char icon_raw[];

WindowAPIMouseMove mouse_move(double x, double y)
{
	printf("%f, %f\n", x, y);
}
WindowAPIMousePress mouse_press(int button, int actions, int mods)
{
}
WindowAPIMouseScroll mouse_scroll(double x, double y)
{
}

WindowAPIKeyPress key_press(int button, int actions, int mods)
{
}
WindowAPIKeyType key_type(int codepoint)
{
}

WindowAPILoop loop()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);

	glVertex2f(-0.5, -0.5);
	glVertex2f(0.5, -0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(-0.5, 0.5);

	glEnd();
}

int main()
{
	puts("Hello world!");

	glfwInit();

	window_t window = { .instance=0, .loop_callback=loop, .mouse_move_callback=mouse_move };

	window_create("UtaSM", 900, 900);

	window_icon_t icon = { .width=128, .height=128, .data=icon_raw };

	window_set_icon(NULL, &icon);
	window_run(&window);

	window_destroy(NULL);

	glfwTerminate();
}
