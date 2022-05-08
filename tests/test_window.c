#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <gamesh/human_input.h>
#include <gamesh/window.h>

#include <log.h>

#include <assert.h>

int onMouseButton(const GamMouseButtonEvent *e);

int main(void) {
	if (!glfwInit()) {
        return 1;
	}
	
	GamWindow *window1 = gam_window_create(GAM_WINDOW_GLFW, 640, 480, "Click this window", NULL, NULL);
	GamWindow *window2 = gam_window_create(GAM_WINDOW_GLFW, 640, 480, "Click this window", NULL, NULL);
	
	gam_window_update(window1);
	gladLoadGL();
	
	
	gam_window_set_on_mouse_button(window1, onMouseButton);
	gam_window_set_on_mouse_button(window2, onMouseButton);
	
	if (!window1 || !window2) {
        glfwTerminate();
        return 1;
    }
	size_t i = 0;
	while (!gam_window_should_close(window1) || !gam_window_should_close(window2))
    {
		gam_window_make_context_current(window1);
        //glClear(GL_COLOR_BUFFER_BIT);
        gam_window_swap_buffers(window1);
		
		gam_window_make_context_current(window2);
        //glClear(GL_COLOR_BUFFER_BIT);
        gam_window_swap_buffers(window2);

        glfwPollEvents();
		gam_window_update(window1);
		gam_window_update(window2);
    }

	gam_window_free(window1);
	gam_window_free(window2);
	glfwTerminate();
	
	return 0;
}



int onMouseButton(const GamMouseButtonEvent *e) {
	gam_window_set_should_close(e->window);
	return 0;
}
