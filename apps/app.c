#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <gamesh/window.h>
#include <gamesh/human_input.h>

#include <stdio.h>

#include <log.h>

#include <pthread.h>


void gam_init_log();
void gam_close_log();
void log_lock_func(bool lock, void *udata);

int onKey(const GamKeyEvent *e);
int onMouseDelta(const GamMouseDeltaEvent *e);
int onMousePos(const GamMousePosEvent *e);
int onMouseButton(const GamMouseButtonEvent *e);

int main(void) {
	gam_init_log();
	log_info("Initializing");
	
	GamWindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = gam_window_create(GAM_WINDOW_GLFW, 640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


	GamInput gamInput;
	gam_input_init(&gamInput);
	gam_input_add_window(&gamInput, window);
	gam_input_add_on_key(&gamInput, onKey);
	gam_input_add_on_mouse_delta(&gamInput, onMouseDelta);
	gam_input_add_on_mouse_pos(&gamInput, onMousePos);
	gam_input_add_on_mouse_button(&gamInput, onMouseButton);

    /* Make the window's context current */
    gam_window_make_context_current(window);
	
	gladLoadGL();

	log_info("Entering app loop");
    /* Loop until the user closes the window */
    while (!gam_window_should_close(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        gam_window_swap_buffers(window);

        /* Poll for and process events */
        glfwPollEvents();
		gam_input_update(&gamInput);
    }

	gam_input_free(&gamInput);

    glfwTerminate();
	
	
	log_info("Exiting");
	gam_close_log();
	
	return 0;
}



// Logging

static pthread_mutex_t gam_log_mutex;
static FILE *log_fp;
void gam_init_log(void) {
	if (pthread_mutex_init(&gam_log_mutex, NULL) != 0)
    {
        log_error("\n mutex init failed\n");
        exit(1);
    }
	log_set_lock(log_lock_func, &gam_log_mutex);

	log_fp = fopen("log.txt", "w+");
	log_add_fp(log_fp, 0);
}

void gam_close_log(void) {
    pthread_mutex_destroy(&gam_log_mutex);
	fclose(log_fp);
}

void log_lock_func(bool lock, void *udata) {
	if(lock) {
		pthread_mutex_lock(udata);
	} else {
		pthread_mutex_unlock(udata);
	}
}

// input logging

int onKey(const GamKeyEvent *e) {
	log_info("Key Event\tkey %d \tscan %d \taction%d \tmods%d",
			 e->key, e->scanCode, e->action, e->mods);
	return 0;
}

int onMouseDelta(const GamMouseDeltaEvent *e) {
	log_info("Mouse delta\tx %f\ty %f", e->xpos, e->ypos);			 
	return 0;
}
int onMousePos(const GamMousePosEvent *e) {
	log_info("Mouse pos\tx %f\ty %f", e->xpos, e->ypos);
	return 0;
}

int onMouseButton(const GamMouseButtonEvent *e) {
	log_info("Mouse button\tbutton %d\taction %d\tmods %d",
			 e->button, e->action, e->mods);
	return 0;
}
