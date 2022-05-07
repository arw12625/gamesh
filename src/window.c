#include <gamesh/window.h>

#include <log.h>

#include <stdlib.h>
#include <string.h>

static int mockWindowCount = 0;


GamWindow* gam_window_create(GamWindowType windowType, int width, int height, const char *title, void *monitor, void *share) {
	GamWindow *window = malloc(sizeof(GamWindow));
	window->windowType = windowType;
	window->_inputAgg = NULL;
	window->onKey = NULL;
	window->onMouseButton = NULL;
	switch(windowType) {
		case GAM_WINDOW_GLFW:
			window->_internal.glfw = glfwCreateWindow(width, height, title, monitor, share);
			glfwSetWindowUserPointer(window->_internal.glfw, window);
			break;
		case GAM_WINDOW_MOCK:
			window->_internal.mock = malloc(sizeof(GamMockWindow));
			GamMockWindow *mock = window->_internal.mock;
			mock->id = mockWindowCount++;
			strncpy(mock->title, title, WINDOW_MAX_TITLE_LENGTH);
			mock->xpos = 0;
			mock->ypos = 0;
			mock->shouldClose = 0;
			break;
	}
	return window;
}

void gam_window_free(GamWindow* window) {
	switch(window->windowType) {
		case GAM_WINDOW_GLFW:
			glfwDestroyWindow(window->_internal.glfw);
			break;
		case GAM_WINDOW_MOCK:
			free(window->_internal.mock);
			break;
	}
	free(window);
}

void gam_window_make_context_current(GamWindow* window) {
	switch(window->windowType) {
		case GAM_WINDOW_GLFW:
			glfwMakeContextCurrent(window->_internal.glfw);
			break;
		case GAM_WINDOW_MOCK:
			break;
	}
}

void gam_window_swap_buffers(GamWindow* window) {
	switch(window->windowType) {
		case GAM_WINDOW_GLFW:
			glfwSwapBuffers(window->_internal.glfw);
			break;
		case GAM_WINDOW_MOCK:
			break;
	}
}

int gam_window_should_close(GamWindow* window) {
	switch(window->windowType) {
		case GAM_WINDOW_GLFW:
			return glfwWindowShouldClose(window->_internal.glfw);
		case GAM_WINDOW_MOCK:
			return window->_internal.mock->shouldClose;
	}
	return 1;
}

void gam_window_get_cursor_pos(GamWindow* window, double *xpos, double *ypos) {
	switch(window->windowType) {
		case GAM_WINDOW_GLFW:
			glfwGetCursorPos(window->_internal.glfw, xpos, ypos);
			break;
		case GAM_WINDOW_MOCK:
			*xpos = window->_internal.mock->xpos;
			*ypos = window->_internal.mock->ypos;
			break;
	}
}


void glfw_on_mouse_button(GLFWwindow* glfwWindow, int button, int action, int mods) {
	void *user = glfwGetWindowUserPointer(glfwWindow);
	if(user == NULL) {
		log_error("GLFW window does not have user set for callbacks");
	}
	GamWindow *window = user;
	GamMouseButtonEvent e;
	e.window = window;
	e.button = button;
	e.action = action;
	e.mods = mods;
	window->onMouseButton(&e);
}


void gam_window_set_on_mouse_button(GamWindow *window, GamOnMouseButton onMouseButton) {
	window->onMouseButton = onMouseButton;
	if(window->windowType == GAM_WINDOW_GLFW) {
		glfwSetMouseButtonCallback(window->_internal.glfw, glfw_on_mouse_button);
	}
}

void glfw_on_key(GLFWwindow* glfwWindow, int key, int scanCode, int action, int mods) {
	void *user = glfwGetWindowUserPointer(glfwWindow);
	if(user == NULL) {
		log_error("GLFW window does not have user set for callbacks");
	}
	GamWindow *window = user;
	GamKeyEvent e;
	e.window = window;
	e.key = key;
	e.scanCode = scanCode;
	e.action = action;
	e.mods = mods;
	window->onKey(&e);
}

void gam_window_set_on_key(GamWindow *window, GamOnKey onKey) {
	window->onKey = onKey;
	if(window->windowType == GAM_WINDOW_GLFW) {
		glfwSetKeyCallback(window->_internal.glfw, glfw_on_key);
	}
}