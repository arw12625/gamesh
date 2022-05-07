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
		gam_input_remove_window(agg, agg->_windows[i]);
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
	log_error("NOT COMPLETELY IMPLEMENTED");
	
	window->_inputAgg = NULL;
	gam_window_set_on_key(window, NULL);
	gam_window_set_on_mouse_button(window, NULL);
	return 1;
}

int gam_input_update(GamInput *agg) {
	// Must call gam_window_update() before
	size_t i, j;
	for(i = 0; i < agg->_numWindows; i++) {
		GamWindow *window = agg->_windows[i];
		if(!gam_window_is_cursor_delta(window)) {
			GamMousePosEvent pe;
			pe.window = window;
			pe.xpos = window->xpos;
			pe.ypos = window->ypos;
			for(j = 0; j < agg->_numMousePosListeners; j++) {
				agg->_mousePosListeners[j](&pe);
			}
		} else {
			GamMouseDeltaEvent de;
			de.window = window;
			de.xdelta = window->xdelta;
			de.ydelta = window->ydelta;
			for(j = 0; j < agg->_numMouseDeltaListeners; j++) {
				agg->_mouseDeltaListeners[j](&de);
			}
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

int gam_input_enable_cursor(GamInput *agg, GamWindow* window, bool enabled) {
	size_t i;
	bool found = false;
	for(i = 0; i < agg->_numWindows; i++) {
		if(window == agg->_windows[i]) {
			found = true;
			break;
		}
	}
	if(!found) {
		log_error("Window not found");
		return 1;
	}
	gam_window_set_cursor_mode(window, enabled);
	return 0;
}

