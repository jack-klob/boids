#ifndef shader_hpp
#define shader_hpp
#include <GL/glew.h>
#include <string>

/// @brief compile a shader
/// @param type the type of shader to compile
/// @param source_code source code for the shader to compile
/// @return the id of the created shader
GLuint compile_shader(GLuint type, const char *source_code);

/// @brief create shader program with vertex and fragment shader
/// @return the id of the created program
GLuint create_shader_program();

#endif