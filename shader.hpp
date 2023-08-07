#ifndef shader_hpp
#define shader_hpp
#include <GL/glew.h>
#include <string>

std::string readFile(const char *file_path);

GLuint compileShader(GLuint type, const char *file_path);

GLuint loadShaders(const char *vertex_path, const char *fragment_path);

#endif