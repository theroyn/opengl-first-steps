#include "pch.h"
#include "Shader.h"


GLint Shader::exit_error()
{
  GLint status = error_ ? R_FAILURE : R_SUCCESS;

  if (error_)
    utility::dbg_print(&((msg_)[0]));

  return status;
}

Shader::Shader(const char *vertex_path, const char *fragment_path) :
  program_(0),
  result_(GL_TRUE)
{
  load_program(result_, msg_, vertex_path, fragment_path);
  error_ = !result_;
}

void Shader::load_program(GLint &result, vector<GLchar>  &msg,
                          const char *vertex_path, const char *fragment_path,
                          const char *geometrt_path)
{
  program_ = utility::load_program(vertex_path, fragment_path, result, msg);
}

void Shader::set_bool(const std::string &name, bool value) const
{
  glUniform1i(glGetUniformLocation(program_, name.c_str()), ( int )value); 
}

void Shader::set_int(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(program_, name.c_str()), value);
}

void Shader::set_float(const std::string &name, float value) const
{
  glUniform1f(glGetUniformLocation(program_, name.c_str()), value);
}

void Shader::set_vec2(const std::string &name, const glm::vec2 &value) const
{
  glUniform2fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}
void Shader::set_vec2(const std::string &name, float x, float y) const
{
  glUniform2f(glGetUniformLocation(program_, name.c_str()), x, y);
}

void Shader::set_vec3(const std::string &name, const glm::vec3 &value) const
{
  glUniform3fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}
void Shader::set_vec3(const std::string &name, float x, float y, float z) const
{
  glUniform3f(glGetUniformLocation(program_, name.c_str()), x, y, z);
}

void Shader::set_vec4(const std::string &name, const glm::vec4 &value) const
{
  glUniform4fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}
void Shader::set_vec4(const std::string &name, float x, float y, float z, float w)
{
  glUniform4f(glGetUniformLocation(program_, name.c_str()), x, y, z, w);
}

void Shader::set_mat2(const std::string &name, const glm::mat2 &mat) const
{
  glUniformMatrix2fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_mat3(const std::string &name, const glm::mat3 &mat) const
{
  glUniformMatrix3fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_mat4(const std::string &name, const glm::mat4 &mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_tex(const std::string &name, const string &path, bool alpha) const
{
  GLuint texture = utility::load_texture(path.c_str(), result_, msg_, alpha);
  error_ = !result_;
}