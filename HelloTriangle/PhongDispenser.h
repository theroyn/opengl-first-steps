#pragma once

#include "Shader.h"
#include <glm.hpp>

struct Material;

class PhongDispenser
{
public:
  PhongDispenser(const Shader &shader, bool is_material = true,
                 glm::vec3 ambient = glm::vec3(0.f),
                 glm::vec3 diffuse = glm::vec3(0.f),
                 glm::vec3 specular = glm::vec3(0.f),
                 int shininess = 64);

  PhongDispenser(const Shader &shader, const Material &material);

  void dispense() const;

  void set_ambient(glm::vec3 ambient);
  void set_diffuse(glm::vec3 diffuse);
  void set_specular(glm::vec3 specular);
  void set_shininess(int shininess);
private:
  glm::vec3 ambient_;
  glm::vec3 diffuse_;
  glm::vec3 specular_;
  int shininess_;

  bool is_material_;

  const Shader &shader_;
};

struct Material
{
  glm::vec3 ambient;
  const string diffuse;
  const string specular;
  int shininess;
};
