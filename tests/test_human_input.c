#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <gamesh/human_input.h>
#include <gamesh/window.h>

#include <log.h>

#include <assert.h>

int testVal = 0;

GamKeyEvent e1;

double testXPos = 10, testYPos = -10;


GamMouseButtonEvent e2;

int onKey(const GamKeyEvent *e);
int onMouseDelta(const GamMouseDeltaEvent *e);
int onMousePos(const GamMousePosEvent *e);
int onMouseButton(const GamMouseButtonEvent *e);

int main(void) {
	GamWindow *window = gam_window_create(GAM_WINDOW_MOCK, 640, 480, "Hello World", NULL, NULL);
	e1.window = window;
	e2.window = window;
	e1.key = 87;
	e1.scanCode = 0;
	e1.action = 1;
	e1.mods = 0;
	e2.button = 1;
	e2.action = 1;
	e2.mods = 0;
	
	GamInput gamInput;
	gam_input_init(&gamInput);
	gam_input_add_window(&gamInput, window);
	gam_input_add_on_key(&gamInput, onKey);
	gam_input_add_on_mouse_delta(&gamInput, onMouseDelta);
	gam_input_add_on_mouse_pos(&gamInput, onMousePos);
	gam_input_add_on_mouse_button(&gamInput, onMouseButton);
	
    gam_window_make_context_current(window);
	
	size_t i = 0;
	while (!gam_window_should_close(window))
    {
        gam_window_swap_buffers(window);

        //glfwPollEvents();
		switch(i % 100) {
			case 30:
				window->onKey(&e1);
				break;
			case 60:
				window->_internal.mock->xpos = testXPos;
				window->_internal.mock->ypos = testYPos;
				break;
			case 61:
				window->_internal.mock->xpos = 0;
				window->_internal.mock->ypos = 0;
				break;
			case 90:
				window->onMouseButton(&e2);
				break;
		}
		if(testVal == 3) {
			window->_internal.mock->shouldClose = 1;
		}
		i++;
		
		gam_input_update(&gamInput);
    }

	gam_input_free(&gamInput);

	gam_window_free(window);
}


// input logging

int onKey(const GamKeyEvent *e) {
	if(testVal == 2 && e->key == 87) {
		testVal++;
	}
	return 0;
}

int onMouseDelta(const GamMouseDeltaEvent *e) {
	return 0;
}

int onMousePos(const GamMousePosEvent *e) {
	if(testVal == 1 && e->xpos == testXPos && e->ypos == testYPos) {
		testVal++;
	}
	return 0;
}

int onMouseButton(const GamMouseButtonEvent *e) {
	if(testVal == 0 && e->button == 1) {
		testVal++;
	}
	return 0;
}
