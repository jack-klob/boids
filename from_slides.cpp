#include <cstdlib>
#include <GLFW/glfw3.h>

void display(GLFWwindow *window)
{
    glClearColor(0.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);
}

int main(int argc, char **argv)
{
    if (!glfwInit())
    {
        return EXIT_FAILURE;
    }
    glfwSwapInterval(1);
    GLFWwindow *window = glfwCreateWindow(512, 512, argv[0], nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowRefreshCallback(window, display);
    while (!glfwWindowShouldClose(window))
    {
        glfwWaitEvents();
    }
    glfwTerminate();
    return EXIT_SUCCESS;
}