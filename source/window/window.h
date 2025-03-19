#ifndef __UTASM_WINDOW_WINDOW_H__
#define __UTASM_WINDOW_WINDOW_H__

#define WindowAPILoop void

#define WindowAPIMouseMove void
#define WindowAPIMousePress void
#define WindowAPIMouseScroll void

#define WindowAPIKeyPress void
#define WindowAPIKeyType void

typedef void (*WindowAPILoopCallback)();

typedef void (*WindowAPIMouseMoveCallback)(double, double);
typedef void (*WindowAPIMousePressCallback)(int, int, int);
typedef void (*WindowAPIMouseScrollCallback)(double, double);

typedef void (*WindowAPIKeyPressCallback)(int, int, int);
typedef void (*WindowAPIKeyTypeCallback)(int);

typedef struct
{
	unsigned int width;
	unsigned int height;

	char* data;
} window_icon_t;

typedef struct
{
	int instance;
	
	WindowAPILoopCallback loop_callback;

	WindowAPIMouseMoveCallback mouse_move_callback;
	WindowAPIMousePressCallback mouse_press_callback;
	WindowAPIMouseScrollCallback mouse_scroll_callback;

	WindowAPIKeyPressCallback key_press_callback;
	WindowAPIKeyTypeCallback key_type_callback;
} window_t;

window_t* window_create(const char* title, int width, int height);
void window_run(window_t* window);
void window_destroy(window_t* window);

void window_set_icon(window_t* window, window_icon_t* icon);

void window_set_loop_callback(window_t* window, WindowAPILoopCallback);

#endif//__UTASM_WINDOW_WINDOW_H__
