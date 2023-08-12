#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

// store shader code as strings so executable does not rely on external files
// more practical for the way the application is being built and executed
inline const char *vertex_source = 
    R"(
        #version 450
        layout(location=0) in vec2 position;
        layout(location=1) in vec2 translate;
        layout(location=2) in mat2 rotation;

        uniform mat4 u_proj;


        void main()
        {
        gl_Position = u_proj * vec4((rotation * position) + translate, 0, 1);
        }
    )";

inline const char *fragment_source =
    R"(
        #version 330 core
        layout(location=0) out vec4 color;

        void main()
        {
            color = vec4(0.5, 1.0, 0.8, 0.8);
        }
    )";

GLuint compile_shader(GLuint type, const char* source_code)
{
    GLuint shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &source_code, nullptr);
    glCompileShader(shaderID);

    int success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        std::cout << "Error compiling shader\n";
        glDeleteShader(shaderID);
        exit(1);
    }

    return shaderID;
}

GLuint create_shader_program()
{
    // need a program to attach the shaders to
    GLuint programID = glCreateProgram();

    // compile the shaders from files
    GLuint vertex_shaderID = compile_shader(GL_VERTEX_SHADER, vertex_source);
    GLuint fragment_shaderID = compile_shader(GL_FRAGMENT_SHADER, fragment_source);

    // attach the shaders to the program
    glAttachShader(programID, vertex_shaderID);
    glAttachShader(programID, fragment_shaderID);

    glLinkProgram(programID);
    glValidateProgram(programID);

    // after being attached from the program the shaders can be deleted
    glDeleteShader(vertex_shaderID);
    glDeleteShader(fragment_shaderID);

    return programID;
}
