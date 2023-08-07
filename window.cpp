#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

    // shader is program which runs on GPU

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f,
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    // selecting is called binding
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // need to put data into buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_DYNAMIC_DRAW);

    // this says that the layout for "positions" is located at location 0
    // layout = 0 can then be accessed from the shader
    glEnableVertexAttribArray(0);
    // describe the layout of the positions buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    unsigned int shader = loadShaders("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    glUseProgram(shader);

    /* Loop until the user closes the window */
    const float increment = 0.0001f;
    while (!glfwWindowShouldClose(window))
    {
        if(positions[2] < 1)
        {
            positions[2] += increment;
        }
        else
        {
            positions[2] = 0.0f;
        }

        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        // can use glBufferSubData to replace data in the vertex buffer
        glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(float), positions);

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll and process events
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}