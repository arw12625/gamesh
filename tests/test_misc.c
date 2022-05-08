#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(void)
{
	#if 0
    GLFWwindow* window1;
    GLFWwindow* window2;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window1 = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window1)
    {
        glfwTerminate();
        return -1;
    }

    /* Create a second windowed mode window and its OpenGL context */
    window2 = glfwCreateWindow(640, 480, "World Hello", NULL, NULL);
    if (!window2)
    {
        glfwTerminate();
        return -1;
    }

        glfwMakeContextCurrent(window1);
	gladLoadGL();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window1) && !glfwWindowShouldClose(window2))
    {
        /* Make the window's context current */
        glfwMakeContextCurrent(window1);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        /* Swap front and back buffers */
        glfwSwapBuffers(window1);

        /* Make the second window's context current */
        glfwMakeContextCurrent(window2);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        /* Swap front and back buffers */
        glfwSwapBuffers(window2);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
	#endif
    return 0;
}