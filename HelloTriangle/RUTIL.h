#ifndef R_GLSL_LOADER_H
#define R_GLSL_LOADER_H

#include "GL/glew.h"
#include <vector>

using namespace std;

GLuint load_program(const char *vertex_path, const char *fragment_path, GLint &result, vector<GLchar> &msg);
GLuint load_shader(const char *path, GLenum type, GLint &result, vector<GLchar> &msg);

void dbg_print(const string &msg);
int r_exit(int code, const string &msg);

#endif