#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/io.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shader.h"

using namespace boost::numeric::ublas;

matrix<float, row_major> make_ortho(const float left, const float right, const float bottom, const float top)
{
    const float near = -1.0f;
    const float far = 1.0f;
    matrix<float, row_major> m(4, 4, 0);
    m(0, 0) = 2.0f / (right - left);
    m(1, 1) = 2.0f / (top - bottom);
    m(2, 2) = -2.f / (far - near);
    m(3,3) = 1;

    m(3,0) = -(right + left) / (right - left);
    m(3,1) = -(top + bottom) / (top - bottom);
    m(3,2) = -(far + near) / (far - near);
    return m;
}

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Flocking Simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

    // shader is program which runs on GPU
    std::cout << glGetString(GL_VERSION) << std::endl;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // defining the shape for a boid
    GLfloat base_tri[] = 
    {
        -10.f, -15.f,
         10.f, -15.f,
         0.f,   10.f,
    };
    
    // buffer for base triangle
    GLuint tri_buffer;
    glGenBuffers(1, &tri_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, tri_buffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), base_tri, GL_STATIC_DRAW);

    // store layout at location 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    GLuint num_boids = 6;
    GLfloat transform[] = {
        400.f, 400.f,
        100.f, 50.f,
        20.f, 700.f,
        700.f, 20.f,
        15.f, 80.f,
        342.f, 516.f,
    };

    // transform buffer
    GLuint trans_buffer;
    glGenBuffers(1, &trans_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, trans_buffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * num_boids * sizeof(GLfloat), transform, GL_STATIC_DRAW);

    // this says that the layout for "transform" is located at location 1
    // layout = 1 can then be accessed from the shader
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    glVertexAttribDivisor(1, 1);

    glBindVertexArray(vao);

    // create and use shaders
    GLuint shader = loadShaders("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    glUseProgram(shader);

    // project to pixel space
    auto proj = make_ortho(0.f, 800.f, 0.f, 800.f);
    GLuint proj_loc = glGetUniformLocation(shader, "u_proj");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &proj(0,0));
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 3, num_boids);

        glfwSwapBuffers(window); // swap front and back buffers
        glfwPollEvents(); // poll and process events
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}