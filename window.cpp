#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "flock.h"
#include "gl_math.h"
#include <chrono>

class FrameLimiter
{
public:
    FrameLimiter(unsigned int fps) : fps_(fps) {}

    bool should_update()
    {
        auto now = std::chrono::steady_clock::now();
        auto dt = now - prev_update;

        if(dt > std::chrono::duration<float>(1.f / fps_))
        {
            prev_update = now;
            return true;
        }

        return false;
    }

private:
    unsigned int fps_;
    std::chrono::steady_clock::time_point prev_update = std::chrono::steady_clock::now();
};

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


    // create and use shaders
    GLuint shader = loadShaders("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    glUseProgram(shader);

    // project to pixel space
    auto proj = make_ortho(0.f, 800.f, 0.f, 800.f);
    GLuint proj_loc = glGetUniformLocation(shader, "u_proj");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &proj[0][0]);

    FrameLimiter limiter(60);
    Flock flock(200);


    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        if(limiter.should_update())
        {
            flock.update();
        }
        flock.draw();

        glfwSwapBuffers(window); // swap front and back buffers
        glfwPollEvents();        // poll and process events
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}