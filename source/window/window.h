#ifndef __UTASM_WINDOW_WINDOW_H__
#define __UTASM_WINDOW_WINDOW_H__

#include "../graphics/glmaid/OpenGL/gl.h"

#define WindowAPICreate int
#define WindowAPIDestroy void

#define WindowAPILoop void

#define WindowAPIResize void

#define WindowAPIMouseMove void
#define WindowAPIMousePress void
#define WindowAPIMouseScroll void

#define WindowAPIKeyPress void
#define WindowAPIKeyType void

typedef struct
{
	GladGLContext* context;

	int width;
	int height;
} window_context_t;

typedef int (*WindowAPICreateCallback)(window_context_t);
typedef void (*WindowAPIDestroyCallback)(window_context_t);

typedef void (*WindowAPILoopCallback)(window_context_t);

typedef void (*WindowAPIResizeCallback)(window_context_t, int, int);

typedef void (*WindowAPIMouseMoveCallback)(double, double);
typedef void (*WindowAPIMousePressCallback)(int, int, int);
typedef void (*WindowAPIMouseScrollCallback)(double, double);

typedef void (*WindowAPIKeyPressCallback)(int, int, int);
typedef void (*WindowAPIKeyTypeCallback)(int);

typedef struct
{
	unsigned int width;
	unsigned int height;

	unsigned char* data;
} window_icon_t;

typedef struct
{
	window_context_t context;

	WindowAPICreateCallback create_callback;
	WindowAPIDestroyCallback destroy_callback;

	WindowAPILoopCallback loop_callback;

	WindowAPIResizeCallback resize_callback;

	WindowAPIMouseMoveCallback mouse_move_callback;
	WindowAPIMousePressCallback mouse_press_callback;
	WindowAPIMouseScrollCallback mouse_scroll_callback;

	WindowAPIKeyPressCallback key_press_callback;
	WindowAPIKeyTypeCallback key_type_callback;
} window_t;

int window_library_init();
void window_library_destroy();

window_t* window_create(const char* title, int width, int height);
void window_run(window_t* window);
void window_destroy(window_t* window);

void window_set_icon(window_t* window, window_icon_t* icon);

void window_set_create_callback(window_t* window, WindowAPICreateCallback callback);
void window_set_destroy_callback(window_t* window, WindowAPIDestroyCallback callback);

void window_set_loop_callback(window_t* window, WindowAPILoopCallback callback);

void window_set_resize_callback(window_t* window, WindowAPIResizeCallback callback);

void window_set_mouse_move_callback(window_t* window, WindowAPIMouseMoveCallback callback);
void window_set_mouse_press_callback(window_t* window, WindowAPIMousePressCallback callback);
void window_set_mouse_scroll_callback(window_t* window, WindowAPIMouseScrollCallback callback);

void window_set_key_press_callback(window_t* window, WindowAPIKeyPressCallback callback);
void window_set_key_type_callback(window_t* window, WindowAPIKeyTypeCallback callback);

#endif//__UTASM_WINDOW_WINDOW_H__
