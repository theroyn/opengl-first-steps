#version 400

in vec3 frag_normal;
in vec3 frag_model_pos;

out vec4 frag_colour;

uniform vec3 object_colour;
uniform vec3 light_colour;
uniform vec3 light_pos;

void main()
{
  float ambient_strength = 0.1;
  vec3 ambient = ambient_strength * light_colour;

  vec3 light_dir = normalize(light_pos - frag_model_pos);
  vec3 norm = normalize(frag_normal);

  float diffuse_strength = max(dot(norm, light_dir), 0);
  vec3 diffuse = diffuse_strength * light_colour;

  vec3 result = (diffuse + ambient) * object_colour;

  frag_colour = vec4(result, 1.0);
}