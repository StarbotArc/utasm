#include "window.h"

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

typedef struct
{
	GLFWwindow* window;
	window_t* instance;
} instancer_t;

GLFWwindow* windows;
instancer_t* instances;

static void mouse_move(GLFWwindow* window, double x, double y)
{
	instances->instance->mouse_move_callback(x, y);
}
static void mouse_press(GLFWwindow* window, int button, int actions, int mods)
{
	instances->instance->mouse_press_callback(button, actions, mods);
}
static void mouse_scroll(GLFWwindow* window, double x, double y)
{
	instances->instance->mouse_scroll_callback(x, y);
}

static void key_press(GLFWwindow* window, int button, int scancode, int actions, int mods)
{
	instances->instance->key_press_callback(button, actions, mods);
}
static void key_type(GLFWwindow* window, unsigned int codepoint)
{
	instances->instance->key_type_callback(codepoint);
}

window_t* window_create(const char* title, int width, int height)
{
	windows = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!windows)
	{
		return NULL;
	}

	glfwMakeContextCurrent(windows);
	glfwSwapInterval(1);
	return NULL;
}
void window_run(window_t* window)
{
	instances = malloc(sizeof *instances);
	instances->window = windows;
	instances->instance = window;

	glfwSetCursorPosCallback(windows, mouse_move);

	while (!glfwWindowShouldClose(windows))
	{
		window->loop_callback();

		glfwSwapBuffers(windows);
		glfwPollEvents();
	}
}
void window_destroy(window_t* window)
{
	glfwDestroyWindow(windows);
}

void window_set_icon(window_t* window, window_icon_t* icon)
{
	GLFWimage glfw_image;
	glfw_image.width = icon->width;
	glfw_image.height = icon->height;
	glfw_image.pixels = icon->data;

	glfwSetWindowIcon(windows, 1, &glfw_image);
}

void window_set_loop_callback(window_t* window, WindowAPILoopCallback callback)
{
	window->loop_callback = callback;
}
