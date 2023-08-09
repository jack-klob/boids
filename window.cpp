#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/io.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shader.hpp"

using namespace boost::numeric::ublas;

matrix<float, row_major> make_ortho(const float left, const float right, const float bottom, const float top)
{
    const float near = -1.0f;
    const float far = 1.0f;
    matrix<float, row_major> m(4, 4, 0);
    m(0, 0) = 2.0f / (right - left);
    m(1, 1) = 2.0f / (top - bottom);
    m(2, 2) = -2.f / (far - near);

    // m(2, 0) = (right + left) / (right - left);
    // m(2, 1) = (top + bottom) / (top - bottom);
    // m(3, 2) = -2.f * far * near / (far - near);

    m(3,0) = -(right + left) / (right - left);
    m(3,1) = -(top + bottom) / (top - bottom);
    m(3,2) = -(far + near) / (far - near);
    m(3,3) = 1;
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

    GLfloat positions[] = {
        20.f, 20.f, 
        80.f, 20.f, 
        80.f, 50.f,
        20.f, 50.f,
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0,
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // vertex buffer object
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), positions, GL_STATIC_DRAW);

    // this says that the layout for "positions" is located at location 0
    // layout = 0 can then be accessed from the shader
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    // index buffer object
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // // project to pixel space
    auto proj = make_ortho(0.f, 800.f, 0.f, 800.f);

    // create and use shaders
    GLuint shader = loadShaders("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    glUseProgram(shader);

    GLuint proj_loc = glGetUniformLocation(shader, "u_proj");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &proj(0,0));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // ibo already bound so last param can be nullptr
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window); // swap front and back buffers
        glfwPollEvents(); // poll and process events
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}