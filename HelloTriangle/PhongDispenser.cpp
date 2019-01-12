#include "pch.h"
#include "PhongDispenser.h"

#define MATERIAL "material."
#define LIGHT std::string("lights")
#define POS "pos"
#define AMBIENT "ambient"
#define DIFFUSE "diffuse"
#define SPECULAR "specular"
#define SHININESS "shininess"
#define CONSTANT "constant_c"
#define LINEAR "linear_c"
#define QUADRATIC "quadratic_c"

#define UPDATE_ERROR(error, result)                        \
                                  if (!(error_))           \
                                    (error_) = !(result_);

using namespace glm;

PhongDispenser::PhongDispenser(const Shader &shader) :
  result_(GL_TRUE),
  shader_(shader)
{}

GLint PhongDispenser::exit_error()
{
  GLint status = error_ ? R_FAILURE : R_SUCCESS;

  if (error_)
    utility::dbg_print(&((msg_)[0]));

  return status;
}
#include <string>
#define LIGHT_IDX(idx) LIGHT + "[" + std::to_string(idx) + "]."

void LightDispenser::dispense() const
{
  string base = LIGHT;

  shader_.use();

  shader_.set_vec4(LIGHT_IDX(idx_) + POS, pos_);

  shader_.set_vec3(LIGHT_IDX(idx_) + AMBIENT, ambient_);
  shader_.set_vec3(LIGHT_IDX(idx_) + DIFFUSE, diffuse_);
  shader_.set_vec3(LIGHT_IDX(idx_) + SPECULAR, specular_);

  shader_.set_float(LIGHT_IDX(idx_) + CONSTANT, constant_c_);
  shader_.set_float(LIGHT_IDX(idx_) + LINEAR, linear_c_);
  shader_.set_float(LIGHT_IDX(idx_) + QUADRATIC, quadratic_c_);
}

LightDispenser::LightDispenser(const Shader &shader,
                               vec4 pos,
                               vec3 ambient,
                               vec3 diffuse,
                               vec3 specular,
                               float constant_c,
                               float linear_c,
                               float quadratic_c) :
  PhongDispenser(shader),
  pos_(pos),
  ambient_(ambient),
  diffuse_(diffuse),
  specular_(specular),
  constant_c_(constant_c),
  linear_c_(linear_c),
  quadratic_c_(quadratic_c) 
{
  idx_ = light_idx++;
}

LightDispenser::LightDispenser(const Shader &shader, const Light &light) :
  LightDispenser(shader, light.pos, light.ambient, light.diffuse, light.specular,
                 light.constant_c, light.linear_c, light.quadratic_c)
{}

LightDispenser::~LightDispenser()
{
  light_idx--;
}

void LightDispenser::set_pos(vec4 pos)
{
  pos_ = pos;
}

void LightDispenser::set_ambient(vec3 ambient)
{
  ambient_ = ambient;
}

void LightDispenser::set_diffuse(vec3 diffuse)
{
  diffuse_ = diffuse;
}

void LightDispenser::set_specular(vec3 specular)
{
  specular_ = specular;
}

MaterialDispenser::MaterialDispenser(const Shader &shader,
                                     const char *diffuse_path,
                                     const char *specular_path,
                                     int shininess) :
  PhongDispenser(shader),
  shininess_(shininess)
{
  shader_.use();

  glActiveTexture(GL_TEXTURE0);

  diffuse_map_ = utility::load_texture(diffuse_path, result_, msg_);

  UPDATE_ERROR(error_, result_);

  glActiveTexture(GL_TEXTURE1);

  specular_map_ = utility::load_texture(specular_path, result_, msg_);

  UPDATE_ERROR(error_, result_)
}

MaterialDispenser::MaterialDispenser(const Shader &shader, const Material &material) :
  PhongDispenser(shader),
  shininess_(material.shininess)
{
  glActiveTexture(GL_TEXTURE0);

  diffuse_map_ = utility::load_texture(material.diffuse.c_str(), result_, msg_);

  UPDATE_ERROR(error_, result_);

  glActiveTexture(GL_TEXTURE1);

  specular_map_ = utility::load_texture(material.specular.c_str(), result_, msg_);

  UPDATE_ERROR(error_, result_);
}


MaterialDispenser::~MaterialDispenser()
{
  if (glIsTexture(diffuse_map_) == GL_TRUE)
    glDeleteTextures(1, &diffuse_map_);

  if (glIsTexture(specular_map_) == GL_TRUE)
    glDeleteTextures(1, &specular_map_);
}

void MaterialDispenser::dispense() const
{
  string base = MATERIAL;

  shader_.use();

  shader_.set_int(base + DIFFUSE, 0);
  shader_.set_int(base + SPECULAR, 1);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, diffuse_map_);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, specular_map_);

  shader_.set_int(base + SHININESS, shininess_);
}

void MaterialDispenser::set_diffuse(const std::string &diffuse)
{
  if (glIsTexture(diffuse_map_) == GL_TRUE)
    glDeleteTextures(1, &diffuse_map_);

  diffuse_map_ = utility::load_texture(diffuse.c_str(), result_, msg_);

  UPDATE_ERROR(error_, result_)
}

void MaterialDispenser::set_specular(const std::string &specular)
{
  if (glIsTexture(specular_map_) == GL_TRUE)
    glDeleteTextures(1, &specular_map_);

  specular_map_ = utility::load_texture(specular.c_str(), result_, msg_);

  UPDATE_ERROR(error_, result_)
}

void MaterialDispenser::set_shininess(int shininess)
{
  shininess_ = shininess;
}