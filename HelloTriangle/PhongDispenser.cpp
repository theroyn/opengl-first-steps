#include "pch.h"
#include "PhongDispenser.h"

#define MATERIAL "material."
#define LIGHT "light."
#define AMBIENT "ambient"
#define DIFFUSE "diffuse"
#define SPECULAR "specular"
#define SHININESS "shininess"

using namespace glm;

void PhongDispenser::dispense() const
{
  string base = is_material_ ? MATERIAL : LIGHT;

  shader_.use();

  shader_.set_vec3(base + AMBIENT, ambient_);
  if (!is_material_) shader_.set_vec3(base + DIFFUSE, diffuse_);
  if (!is_material_) shader_.set_vec3(base + SPECULAR, specular_);

  if (is_material_)
    shader_.set_int(base + SHININESS, shininess_);

}

PhongDispenser::PhongDispenser(const Shader &shader, bool is_material,
                               vec3 ambient, vec3 diffuse,
                               vec3 specular, int shininess) :
  shader_(shader),
  is_material_(is_material),
  ambient_(ambient),
  diffuse_(diffuse),
  specular_(specular),
  shininess_(shininess) {}

PhongDispenser::PhongDispenser(const Shader &shader, const Material &material) :
  PhongDispenser(shader, true, material.ambient, material.diffuse, material.specular, material.shininess)
{}

void PhongDispenser::set_ambient(vec3 ambient)
{
  ambient_ = ambient;
}

void PhongDispenser::set_diffuse(vec3 diffuse)
{
  diffuse_ = diffuse;
}

void PhongDispenser::set_specular(vec3 specular)
{
  specular_ = specular;
}

void PhongDispenser::set_shininess(int shininess)
{
  shininess_ = shininess;
}