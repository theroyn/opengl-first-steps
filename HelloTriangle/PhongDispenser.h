#pragma once

#include "Shader.h"
#include <glm.hpp>

struct Material;
struct Light;

class PhongDispenser
{
public:
  PhongDispenser(const Shader &shader);

  virtual void dispense() const = 0;

  bool error() const { return error_; }
  GLint exit_error();

protected:
  GLint result_;
  vector<GLchar> msg_;
  bool error_;

  const Shader &shader_;
};

class MaterialDispenser : public PhongDispenser
{
public:
  MaterialDispenser(const Shader &shader, const Material &material);
  MaterialDispenser(const Shader &shader,
                    const char *diffuse_path,
                    const char *specular_path,
                    int shininess = 64);

  ~MaterialDispenser();

  void dispense() const;

  void set_diffuse(const std::string &diffuse);
  void set_specular(const std::string &specular);
  void set_shininess(int shininess);

private:
  int shininess_;

  GLuint diffuse_map_;
  GLuint specular_map_;

};

static int light_idx = 0;

class LightDispenser : public PhongDispenser
{
public:
  LightDispenser(const Shader &shader,
                 glm::vec4 pos = glm::vec4(0.f),
                 glm::vec3 ambient = glm::vec3(0.f),
                 glm::vec3 diffuse = glm::vec3(0.f),
                 glm::vec3 specular = glm::vec3(0.f),
                 float constant_c = 1.f, float linear_c = 1.f, float quadratic_c = 1.f);

  //PhongDispenser(const Shader &shader, const Material &material);

  LightDispenser(const Shader &shader, const Light &light);

  ~LightDispenser();

  void dispense() const;

  void set_pos(glm::vec4 pos);

  void set_ambient(glm::vec3 ambient);
  void set_diffuse(glm::vec3 diffuse);
  void set_specular(glm::vec3 specular);
private:
  glm::vec4 pos_;

  glm::vec3 ambient_;
  glm::vec3 diffuse_;
  glm::vec3 specular_;

  float constant_c_;
  float linear_c_;
  float quadratic_c_;

  int idx_;
};

struct Material
{
  const string diffuse;
  const string specular;
  int shininess;
};

struct Light
{
  glm::vec4 pos;

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float constant_c;
  float linear_c;
  float quadratic_c;
};
