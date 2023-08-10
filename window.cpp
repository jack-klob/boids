#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "flock.h"
#include "gl_math.h"

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(800, 800, "Flocking Simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    // this currently must be done outside of boids class
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    Flock flock(100);
    flock.createData();

    // create and use shaders
    GLuint shader = loadShaders("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    glUseProgram(shader);

    // project to pixel space
    auto proj = make_ortho(0.f, 800.f, 0.f, 800.f);
    GLuint proj_loc = glGetUniformLocation(shader, "u_proj");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &proj(0,0));

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        flock.draw();

        glfwSwapBuffers(window); // swap front and back buffers
        glfwPollEvents(); // poll and process events
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}