#ifndef R_GLSL_LOADER_H
#define R_GLSL_LOADER_H

#define R_FAILURE -1
#define R_SUCCESS 0

#define r_handle_result(result, msg)                            \
                                      if (!(result))            \
                                      {                         \
                                        dbg_print(&((msg)[0])); \
                                        return R_FAILURE;       \
                                      }

#include "GL/glew.h"
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <vector>
namespace utility
{
  using namespace std;

  GLuint load_program(GLuint      vertShader, GLuint          fragShader,
                      GLint      &result, vector<GLchar> &msg);
  GLuint load_program(const char *vertex_path, const char     *fragment_path,
                      GLint      &result, vector<GLchar> &msg,
                      GLuint     *vs = NULL, GLuint         *fs = NULL);
  GLuint load_shader(const char *path, GLenum type, GLint &result, vector<GLchar> &msg);
  GLuint load_texture(const char *path, GLint &result, vector<GLchar> &msg/*, bool has_alpha = false*/);

  void dbg_print(const string &msg);
  int r_exit(int code, const string &msg = "");
  void r_update_fps_counter(GLFWwindow* window);
}



#endif // R_GLSL_LOADER_H