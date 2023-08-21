#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "flock.h"
#include "gl_math.h"
#include "utils.h"

int main(int argc, char* argv[])
{
    Flock::parameters params = handle_arguments(argc, argv);

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(params.width, params.height, "Flocking Simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // required for lab computers
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

    // create and use shaders
    GLuint shader_program = create_shader_program();
    glUseProgram(shader_program);

    // project to pixel space
    auto proj = make_ortho(0.f, params.width, 0.f, params.height);
    GLuint proj_loc = glGetUniformLocation(shader_program, "u_proj");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &proj[0][0]);

    FrameLimiter limiter(60);
    Flock flock(params);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        if(limiter.should_update())
        {
            flock.update(limiter.frame_dt());
        }
        flock.draw();

        glfwSwapBuffers(window); // swap front and back buffers
        glfwPollEvents();        // poll and process events
    }

    glDeleteProgram(shader_program);
    glfwTerminate();
    return 0;
}