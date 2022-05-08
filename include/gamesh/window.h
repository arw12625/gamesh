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
	bool cursorToChange, cursorEnabled;
	double xpos, ypos;
	double xdelta, ydelta;
	union GamInternalWindow {
		GLFWwindow* glfw;
		GamMockWindow* mock;
	} _internal;
} GamWindow;

/**
 * @brief Create a window
 *
 * Create a window with type @p windowType and size @p width x @p height. 
 * Type @p windowType can be GAM_WINDOW_GLFW for glfw windows,
 * or GAM_WINDOW_MOCK for simulated/mock windows.
 * @param window Pointer to the window
 * @param width Width of the window
 * @param height Height of the window
 * @param title Title of window as a string 
 * @param monitor Unused
 * @param share Unused
 * @warning Must be called from GLFW main thread for GLFW windows.
 */
GamWindow* gam_window_create(GamWindowType windowType, int width, int height, const char *title, void *monitor, void *share);

/**
 * @brief Free the window
 *
 * Close and free the memory of the window @p window.
 * @param window Pointer to the window
 * @warning Must be called from GLFW main thread for GLFW windows.
 * @warning Context must not be possessed by non-main threads.
 */
void gam_window_free(GamWindow *window);

/**
 * @brief Update the window
 *
 * Update the window @p window. Necessary for processing events.
 * @param window Pointer to the window
 * @warning Must be called from GLFW main thread for GLFW windows.
 */
void gam_window_update(GamWindow *window);

/**
 * @brief Assign the openGL context to the calling thread.
 *
 * Assign the openGL context of the window @p window to the calling thread.
 * Can be called from any thread.
 * @param window Pointer to the window
 */
void gam_window_make_context_current(GamWindow* window);

/**
 * @brief Notify the window it should swap its front/back buffers.
 *
 * Notify the window @p window it should swap its front/back buffers.
 * Can be called from any thread.
 * @param window Pointer to the window
 */
void gam_window_swap_buffers(GamWindow *window);

/**
 * @brief Check if window should close.
 *
 * Check if window @p window should close.
 * Can be called from any thread.
 * @param window Pointer to the window
 */
int gam_window_should_close(GamWindow *window);


/**
 * @brief Notify the window it should close.
 *
 * Notify the window @p window to close.
 * Can be called from any thread.
 * @param window Pointer to the window
 */
void gam_window_set_should_close(GamWindow* window);


/**
 * @brief Set the mouse button callback for the window.
 *
 * Set the mouse button callback @p onMouseButton for the window @p window.
 * @param window Pointer to the window
 * @param onMouseButton The callback
 * @warning Must be called from GLFW main thread for GLFW windows.
 */
void gam_window_set_on_mouse_button(GamWindow *window, GamOnMouseButton onMouseButton);

/**
 * @brief Set the key callback for the window.
 *
 * Set the key callback @p onKey for the window @p window.
 * @param window Pointer to the window
 * @param onKey The callback
 * @warning Must be called from GLFW main thread for GLFW windows.
 */
void gam_window_set_on_key(GamWindow *window, GamOnKey onKey);

/**
 * @brief Check if the current cursor position in the window is a delta.
 *
 * Check if the current cursor position in the window @p window is a delta,
 * i.e., whether it corresponds to raw movement, or is given in screen coordinates.
 * @param window Pointer to the window
 * @return Whether the current cursor position is actually a delta
 */
bool gam_window_is_cursor_delta(GamWindow *window);

/**
 * @brief Set the mode of the window's cursor (enabled/disabled)
 *
 * Enable or disable the cursor of the window @p window based on @p enabled.
 * In enabled mode, the cursor is visible and it's position on the screen is reported.
 * In disabled mode, the cursor is hidden and the raw motion of the mouse is reported.
 * This does not affect the current cursor data stored in the window. See gam_window_is_cursor_delta()
 * @param window Pointer to the window
 * @param enabled Whether or not to enable
 */
void gam_window_set_cursor_mode(GamWindow *window, bool enabled);

#endif
/* WINDOW_H */