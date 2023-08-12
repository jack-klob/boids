#ifndef shader_hpp
#define shader_hpp
#include <GL/glew.h>
#include <string>

/// @brief read data from a file
/// @param file_path the path of the file to read
/// @return a string containing the file data
std::string readFile(const char *file_path);

/// @brief compile a shader
/// @param type the type of shader to compile
/// @param file_path the path of the file containing the shader code
/// @return the id of the created shader
GLuint compileShader(GLuint type, const char *file_path);

/// @brief load and use shaders
/// @param vertex_path path to vertex shader code
/// @param fragment_path path to fragment shader code
/// @return the id of the created program
GLuint loadShaders(const char *vertex_path, const char *fragment_path);

#endif