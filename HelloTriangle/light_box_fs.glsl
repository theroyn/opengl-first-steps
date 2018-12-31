#version 400

struct Material
{
  vec3 ambient;
  sampler2D diffuse;
  sampler2D specular;
  //vec3 specular;
  int shininess;
};

struct Light
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

// View oriented
in vec3 frag_view_normal;
in vec3 frag_view_pos;
in vec2 frag_tex;

out vec4 frag_colour;

uniform vec3 light_pos;
uniform vec3 light_view_pos;

uniform Material material;
uniform Light light;

void main()
{
  vec3 material_ambient_diffuse = texture(material.diffuse, frag_tex).rgb;
  vec3 material_specular = vec3(texture(material.specular, frag_tex));

  vec3 ambient = material_ambient_diffuse * light.ambient;

  vec3 light_dir = normalize(light_view_pos - frag_view_pos);
  vec3 norm = normalize(frag_view_normal);
  float diffuse_strength = max(dot(norm, light_dir), 0);
  vec3 diffuse = material_ambient_diffuse * diffuse_strength * light.diffuse;
  
  vec3 r = -light_dir + ((2 * dot(light_dir, norm)) * norm);
  vec3 eye_dir = -normalize(frag_view_pos);
  float spec = pow(max(dot(r, eye_dir), 0.f), material.shininess);
  vec3 specular = material_specular * spec * light.specular;
  //vec3 specular = material.specular * spec * light.specular;

  vec3 result = specular + diffuse + ambient;

  frag_colour = vec4(result, 1.0);
}