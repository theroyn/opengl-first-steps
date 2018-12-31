#pragma once

#include "RUTIL.h"

#include <glm.hpp>

using namespace std;
class Shader
{
public:
  Shader(const char *vertex_path, const char     *fragment_path);
  void load_program(GLint      &result, vector<GLchar> &msg,
                      const char *vertex_path, const char     *fragment_path,
                      const char *geometrt_path = nullptr);
  bool error() const { return error_; }
  GLint exit_error();

  void use() const { glUseProgram(program_); }

  void set_bool(const std::string &name, bool value) const;
  void set_int(const std::string &name, int value) const;
  void set_float(const std::string &name, float value) const;
  void set_vec2(const std::string &name, const glm::vec2 &value) const;
  void set_vec2(const std::string &name, float x, float y) const;
  void set_vec3(const std::string &name, const glm::vec3 &value) const;
  void set_vec3(const std::string &name, float x, float y, float z) const;
  void set_vec4(const std::string &name, const glm::vec4 &value) const;
  void set_vec4(const std::string &name, float x, float y, float z, float w);
  void set_mat2(const std::string &name, const glm::mat2 &mat) const;
  void set_mat3(const std::string &name, const glm::mat3 &mat) const;
  void set_mat4(const std::string &name, const glm::mat4 &mat) const;

  void set_tex(const std::string &name, const string &path, bool alpha = false) const;

private:
  GLuint program_;
  GLint result_;
  vector<GLchar> msg_;
  bool error_;
};
