#ifndef WINDOW_H
#define WINDOW_H

#include <gamesh/human_input.h>

#include <GLFW/glfw3.h>

#define WINDOW_MAX_TITLE_LENGTH 256

// Type declarations from <human_input.h>
typedef struct GamKeyEvent GamKeyEvent;
typedef struct GamMouseDeltaEvent GamMouseDeltaEvent;
typedef struct GamMousePosEvent GamMousePosEvent;
typedef struct GamMouseButtonEvent GamMouseButtonEvent;
typedef struct GamInput GamInput;
typedef int (*GamOnKey)(const GamKeyEvent *e);
typedef int (*GamOnMouseDelta)(const GamMouseDeltaEvent *e);
typedef int (*GamOnMousePos)(const GamMousePosEvent *e);
typedef int (*GamOnMouseButton)(const GamMouseButtonEvent *e);


typedef enum GamWindowType {
	GAM_WINDOW_GLFW, GAM_WINDOW_MOCK
} GamWindowType;

typedef struct GamMockWindow {
	char title[WINDOW_MAX_TITLE_LENGTH];
	int id;
	double xpos, ypos;
	int shouldClose;
} GamMockWindow;

typedef struct GamWindow {
	GamWindowType windowType;
	GamInput *_inputAgg;
	GamOnKey onKey;
	GamOnMouseButton onMouseButton;	
	union GamInternalWindow {
		GLFWwindow* glfw;
		GamMockWindow* mock;
	} _internal;
} GamWindow;

GamWindow* gam_window_create(GamWindowType windowType, int width, int height, const char *title, void *monitor, void *share);
void gam_window_free(GamWindow* window);

void gam_window_make_context_current(GamWindow* window);
void gam_window_swap_buffers(GamWindow* window);
int gam_window_should_close(GamWindow* window);

void gam_window_set_on_mouse_button(GamWindow *window, GamOnMouseButton onMouseButton);
void gam_window_set_on_key(GamWindow *window, GamOnKey onMouseButton);
void gam_window_get_cursor_pos(GamWindow* window, double *xpos, double *ypos);

#endif
/* WINDOW_H */