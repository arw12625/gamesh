#ifndef HUMAN_INPUT_H
#define HUMAN_INPUT_H

#include <gamesh/con_queue.h>
#include <gamesh/window.h>

#include <stdbool.h>

#define HUMAN_INPUT_MAX_NUM_LISTENERS 32
#define HUMAN_INPUT_MAX_NUM_WINDOWS 8


// Type declarations from <window.h>
typedef struct GamWindow GamWindow;



typedef struct GamKeyEvent {
	GamWindow* window;
	int key;
	int scanCode;
	int action;
	int mods;
} GamKeyEvent;

typedef struct GamMouseDeltaEvent {
	GamWindow* window;
	double xdelta;
	double ydelta;
} GamMouseDeltaEvent;

typedef struct GamMousePosEvent {
	GamWindow* window;
	double xpos;
	double ypos;
} GamMousePosEvent;

typedef struct GamMouseButtonEvent {
	GamWindow* window;
	int button;
	int action;
	int mods;
} GamMouseButtonEvent;


typedef int (*GamOnKey)(const GamKeyEvent *e);
typedef int (*GamOnMouseDelta)(const GamMouseDeltaEvent *e);
typedef int (*GamOnMousePos)(const GamMousePosEvent *e);
typedef int (*GamOnMouseButton)(const GamMouseButtonEvent *e);

typedef struct GamInput {
	ConQueue _keyQueue;
	GamOnKey _keyListeners[HUMAN_INPUT_MAX_NUM_LISTENERS];
	size_t _numKeyListeners;

	GamOnMouseDelta _mouseDeltaListeners[HUMAN_INPUT_MAX_NUM_LISTENERS];
	size_t _numMouseDeltaListeners;
	
	GamOnMousePos _mousePosListeners[HUMAN_INPUT_MAX_NUM_LISTENERS];
	size_t _numMousePosListeners;
	
	ConQueue _mouseButtonQueue;
	GamOnMouseButton _mouseButtonListeners[HUMAN_INPUT_MAX_NUM_LISTENERS];
	size_t _numMouseButtonListeners;
	
	GamWindow* _windows[HUMAN_INPUT_MAX_NUM_WINDOWS];
	size_t _numWindows;
} GamInput;

/**
 * @brief Initialize the input aggregator.
 *
 * Initialize the input aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_input_init(GamInput *agg);

/**
 * @brief Free the input aggregator.
 *
 * Free the input aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 * @warning Must be called from GLFW main thread for GLFW windows.
 */
int gam_input_free(GamInput *agg);

/**
 * @brief Add a window to the aggregator
 *
 * Add the window at @p window to route inputs through the aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @param window Pointer to the window.
 * @return Returns 0 if successful, nonzero otherwise.
 * @warning Must be called from GLFW main thread for GLFW windows.
 */
int gam_input_add_window(GamInput *agg, GamWindow *window);


/**
 * @brief Remove a window from the aggregator
 *
 * Add the window at @p window to route inputs through the aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @param window Pointer to the window.
 * @return Returns 0 if successful, nonzero otherwise.
 * @warning Must be called from GLFW main thread for GLFW windows.
 */
int gam_input_remove_window(GamInput *agg, GamWindow *window);


/**
 * @brief Update input aggregator to process input commands.
 *
 * Update the input aggregator at @p agg to process inputs.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_input_update(GamInput *agg);

/**
 * @brief Add a listener to the event aggregator.
 *
 * Add a listener @p on_key_event to the event aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @param onKey Pointer to the listener as a function.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_input_add_on_key(GamInput *agg, GamOnKey onKey);


/**
 * @brief Add a listener to the event aggregator.
 *
 * Add a listener @p on_mouse_pos_event to the event aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @param onMouseDelta Pointer to the listener as a function.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_input_add_on_mouse_delta(GamInput *agg, GamOnMouseDelta onMouseDelta);


/**
 * @brief Add a listener to the event aggregator.
 *
 * Add a listener @p on_mouse_pos_event to the event aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @param onMousePos Pointer to the listener as a function.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_input_add_on_mouse_pos(GamInput *agg, GamOnMousePos onMousePos);


/**
 * @brief Add a listener to the event aggregator.
 *
 * Add a listener @p on_key_event to the event aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @param onMouseButton Pointer to the listener as a function.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_input_add_on_mouse_button(GamInput *agg, GamOnMouseButton onMouseButton);


/**
 * @brief Enable the cursor of the window.
 *
 * @param agg Pointer to the aggregator.
 * @param window Pointer to the window.
 * @param enabled Whether or not to enable.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_input_enable_cursor(GamInput *agg, GamWindow* window, bool enabled);

// TODO Add methods for removing listeners

#endif
/* HUMAN_INPUT_H */