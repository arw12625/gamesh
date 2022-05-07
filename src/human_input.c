#include <gamesh/human_input.h>

#include <log.h>

int gam_input_init(GamInput *agg) {
	if(con_queue_init(&agg->_keyQueue, sizeof(GamKeyEvent)) != 0) return 1;
	
	if(con_queue_init(&agg->_mouseButtonQueue, sizeof(GamMouseButtonEvent))) return 1;
	
	agg->_numKeyListeners = 0;
	agg->_numMousePosListeners = 0;
	agg->_numMouseDeltaListeners = 0;
	agg->_numMouseButtonListeners = 0;
	agg->_numWindows = 0;
	return 0;
}


int gam_input_free(GamInput *agg) {
	size_t i;
	for(i = 0; i < agg->_numWindows; i++) {
		agg->_windows[i]->_inputAgg = NULL;
	}
	int r1 = con_queue_free(&agg->_keyQueue);
	int r4 = con_queue_free(&agg->_mouseButtonQueue);
	if(r1 != 0 || r4 != 0) {
		return 1;
	} else {
		return 0;
	}
}

int gam_input_on_key(const GamKeyEvent *e) {
	if(e->window->_inputAgg == NULL) {
		log_error("Could not recover input aggregator from window");
		return 1;
	}
	if(con_queue_enqueue(&e->window->_inputAgg->_keyQueue, e) != 0) {
		log_error("Could not queue key event");
		return 1;
	}
	return 0;
}

int gam_input_on_mouse_button(const GamMouseButtonEvent *e) {
	if(e->window->_inputAgg == NULL) {
		log_error("Could not recover input aggregator from window");
		return 1;
	}
	if(con_queue_enqueue(&e->window->_inputAgg->_mouseButtonQueue, e) != 0) {
		log_error("Could not queue mouse button event");
		return 1;
	}
	return 0;
}

int gam_input_add_window(GamInput *agg, GamWindow *window) {
		
	agg->_windows[agg->_numWindows++] = window;
	window->_inputAgg = agg;
	
	gam_window_set_on_key(window, gam_input_on_key);
	gam_window_set_on_mouse_button(window, gam_input_on_mouse_button);
	return 0;
}

int gam_input_remove_window(GamInput *agg, GamWindow *window) {
	log_error("NOT IMPLEMENTED");
	return 1;
}

int gam_input_update(GamInput *agg) {
	// Must call glfwPollEvents() before
	size_t i, j;
	for(i = 0; i < agg->_numWindows; i++) {
		double xpos, ypos;
		GamMousePosEvent pe;
		GamMouseDeltaEvent de;
		gam_window_get_cursor_pos(agg->_windows[i], &xpos, &ypos);
		pe.window = agg->_windows[i];
		pe.xpos = xpos;
		pe.ypos = ypos;
		de.window = agg->_windows[i];
		de.xpos = xpos;
		de.ypos = ypos;
		
		for(j = 0; j < agg->_numMouseDeltaListeners; j++) {
			agg->_mouseDeltaListeners[j](&de);
		}
		for(j = 0; j < agg->_numMousePosListeners; j++) {
			agg->_mousePosListeners[j](&pe);
		}
	}
	GamKeyEvent ke;
	while(con_queue_dequeue(&agg->_keyQueue, &ke) == 0) {
		for(j = 0; j < agg->_numKeyListeners; j++) {
			agg->_keyListeners[j](&ke);
		}
	}
	
	GamMouseButtonEvent be;
	while(con_queue_dequeue(&agg->_mouseButtonQueue, &be) == 0) {
		for(j = 0; j < agg->_numMouseButtonListeners; j++) {
			agg->_mouseButtonListeners[j](&be);
		}
	}
}


int gam_input_add_on_key(GamInput *agg, GamOnKey onKey) {
	if(agg->_numKeyListeners >= HUMAN_INPUT_MAX_NUM_LISTENERS) return 1;
	agg->_keyListeners[agg->_numKeyListeners++] = onKey;
	return 0;
}

int gam_input_add_on_mouse_delta(GamInput *agg, GamOnMouseDelta onMouseDelta) {
	if(agg->_numMouseDeltaListeners >= HUMAN_INPUT_MAX_NUM_LISTENERS) return 1;
	agg->_mouseDeltaListeners[agg->_numMouseDeltaListeners++] = onMouseDelta;
	return 0;
}

int gam_input_add_on_mouse_pos(GamInput *agg, GamOnMousePos onMousePos) {
	if(agg->_numMousePosListeners >= HUMAN_INPUT_MAX_NUM_LISTENERS) return 1;
	agg->_mousePosListeners[agg->_numMousePosListeners++] = onMousePos;
	return 0;
}

int gam_input_add_on_mouse_button(GamInput *agg, GamOnMouseButton onMouseButton) {
	if(agg->_numMouseButtonListeners >= HUMAN_INPUT_MAX_NUM_LISTENERS) return 1;
	agg->_mouseButtonListeners[agg->_numMouseButtonListeners++] = onMouseButton;
	return 0;
}


