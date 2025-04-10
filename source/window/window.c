#include "window.h"

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

typedef struct
{
	GLFWwindow* window;
	window_t* instance;
} instancer_t;

static instancer_t* instances;
static unsigned int instance_size;
static unsigned int instance_alloc;

/* Placeholder events */

static WindowAPICreate pl_create() { return 0; }
static WindowAPIDestroy pl_destroy() {}

static WindowAPILoop pl_loop() {}

static WindowAPIResize pl_resize() {}

static WindowAPIMouseMove pl_mouse_move() {}
static WindowAPIMousePress pl_mouse_press() {}
static WindowAPIMouseScroll pl_mouse_scroll() {}

static WindowAPIKeyPress pl_key_press() {}
static WindowAPIKeyType pl_key_type() {}

/* Utilities functions */

static GLFWwindow* find_glfw_window_with_native_window(window_t* ptr)
{
	for (int i = 0; i < instance_size; i++)
	{
		if (instances[i].instance == ptr)
		{
			return instances[i].window;
		}
	}
	return NULL;
}

static window_t* find_native_window_with_glfw_window(GLFWwindow* ptr)
{
	for (int i = 0; i < instance_size; i++)
	{
		if (instances[i].window == ptr)
		{
			return instances[i].instance;
		}
	}
	return NULL;
}

/* Event links */

static void resize(GLFWwindow* window, int width, int height)
{
	window_t* native_window = find_native_window_with_glfw_window(window);
	native_window->resize_callback(native_window->context, width, height);
	native_window->context.width = width;
	native_window->context.height = height;
}

static void mouse_move(GLFWwindow* window, double x, double y)
{
	find_native_window_with_glfw_window(window)->mouse_move_callback(x, y);
}
static void mouse_press(GLFWwindow* window, int button, int actions, int mods)
{
	find_native_window_with_glfw_window(window)->mouse_press_callback(button, actions, mods);
}
static void mouse_scroll(GLFWwindow* window, double x, double y)
{
	find_native_window_with_glfw_window(window)->mouse_scroll_callback(x, y);
}

static void key_press(GLFWwindow* window, int button, int scancode, int actions, int mods)
{
	find_native_window_with_glfw_window(window)->key_press_callback(button, actions, mods);
}
static void key_type(GLFWwindow* window, unsigned int codepoint)
{
	find_native_window_with_glfw_window(window)->key_type_callback(codepoint);
}

/* Visible functions */

int window_library_init()
{
	if (!glfwInit()) return 1;
	return 0;
}

void window_library_destroy()
{
	glfwTerminate();
}

window_t* window_create(const char* title, int width, int height)
{
	window_t* native_window = malloc(sizeof *native_window);

	if (!native_window)
	{
		return NULL;
	}

	native_window->create_callback = pl_create;
	native_window->destroy_callback = pl_destroy;

	native_window->loop_callback = pl_loop;

	native_window->resize_callback = pl_resize;

	native_window->mouse_move_callback = pl_mouse_move;
	native_window->mouse_press_callback = pl_mouse_press;
	native_window->mouse_scroll_callback = pl_mouse_scroll;

	native_window->key_press_callback = pl_key_press;
	native_window->key_type_callback = pl_key_type;

	native_window->context.width = width;
	native_window->context.height = height;

	GLFWwindow* glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!glfw_window)
	{
		free(native_window);

		return NULL;
	}

	glfwMakeContextCurrent(glfw_window);

	GladGLContext* context = malloc(sizeof *context);
	if (!context)
	{
		free(native_window);
		glfwDestroyWindow(glfw_window);

		return NULL;
	}

	int version = gladLoadGLContext(context, glfwGetProcAddress);
	printf("Created OpenGL %d.%d instance.\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
	fflush(stdout);

	native_window->context.context = context;

	glfwSwapInterval(1);

	instances = malloc(sizeof *instances);
	instances[instance_size].window = glfw_window;
	instances[instance_size].instance = native_window;
	instance_size++;

	return native_window;
}
void window_run(window_t* window)
{
	GLFWwindow* glfw_window = find_glfw_window_with_native_window(window);

	if (window->create_callback(window->context)) return;

	glfwSetFramebufferSizeCallback(glfw_window, resize);

	glfwSetCursorPosCallback(glfw_window, mouse_move);
	glfwSetMouseButtonCallback(glfw_window, mouse_press);
	glfwSetScrollCallback(glfw_window, mouse_scroll);

	glfwSetKeyCallback(glfw_window, key_press);
	glfwSetCharCallback(glfw_window, key_type);

	while (!glfwWindowShouldClose(glfw_window))
	{
		window->loop_callback(window->context);

		glfwSwapBuffers(glfw_window);
		glfwPollEvents();
	}

	window->destroy_callback(window->context);
}
void window_destroy(window_t* window)
{
	GLFWwindow* glfw_window = find_glfw_window_with_native_window(window);

	free(window->context.context);

	glfwDestroyWindow(glfw_window);
	free(window);
}

void window_set_icon(window_t* window, window_icon_t* icon)
{
	GLFWwindow* glfw_window = find_glfw_window_with_native_window(window);

	GLFWimage glfw_image;
	glfw_image.width = icon->width;
	glfw_image.height = icon->height;
	glfw_image.pixels = icon->data;

	glfwSetWindowIcon(glfw_window, 1, &glfw_image);
}

void window_set_create_callback(window_t* window, WindowAPICreateCallback callback)
{
	window->create_callback = callback;
}
void window_set_destroy_callback(window_t* window, WindowAPIDestroyCallback callback)
{
	window->destroy_callback = callback;
}

void window_set_loop_callback(window_t* window, WindowAPILoopCallback callback)
{
	window->loop_callback = callback;
}

void window_set_resize_callback(window_t* window, WindowAPIResizeCallback callback)
{
	window->resize_callback = callback;
}

void window_set_mouse_move_callback(window_t* window, WindowAPIMouseMoveCallback callback)
{
	window->mouse_move_callback = callback;
}
void window_set_mouse_press_callback(window_t* window, WindowAPIMousePressCallback callback)
{
	window->mouse_press_callback = callback;
}
void window_set_mouse_scroll_callback(window_t* window, WindowAPIMouseScrollCallback callback)
{
	window->mouse_scroll_callback = callback;
}

void window_set_key_press_callback(window_t* window, WindowAPIKeyPressCallback callback)
{
	window->key_press_callback = callback;
}
void window_set_key_type_callback(window_t* window, WindowAPIKeyTypeCallback callback)
{
	window->key_type_callback = callback;
}
