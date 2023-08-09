#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string readFile(const char* file_path)
{
    // read in data from provided path
    std::ifstream file(file_path);
    std::string line;
    std::stringstream ss;

    while (getline(file, line))
    {
        ss << line << '\n';
    }

    return ss.str();
}

GLuint compileShader(GLuint type, const char* file_path)
{
    std::string data = readFile(file_path);
    const char *shader_src = data.c_str();

    GLuint shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &shader_src, nullptr);
    glCompileShader(shaderID);

    int success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        std::cout << "Error compiling shader from path: " << file_path << '\n';
        glDeleteShader(shaderID);
        exit(1);
    }

    return shaderID;
}

GLuint loadShaders(const char *vertex_path, const char *fragment_path)
{
    // need a program to attach the shaders to
    GLuint programID = glCreateProgram();

    // compile the shaders from files
    GLuint vertex_shaderID = compileShader(GL_VERTEX_SHADER, vertex_path);
    GLuint fragment_shaderID = compileShader(GL_FRAGMENT_SHADER, fragment_path);

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
