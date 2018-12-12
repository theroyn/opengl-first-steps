#version 400

// Model oriented
in vec3 frag_normal;
in vec3 frag_model_pos;

// View oriented
in vec3 frag_view_normal;
in vec3 frag_view_pos;

out vec4 frag_colour;

uniform vec3 object_colour;
uniform vec3 light_colour;
uniform vec3 light_pos;
uniform vec3 light_view_pos;

void main()
{
  float ambient_strength = 0.15;
  vec3 ambient = ambient_strength * light_colour;


  vec3 v_light_dir = normalize(light_view_pos - frag_view_pos);
  vec3 v_norm = normalize(frag_view_normal);
  float v_diffuse_strength = max(dot(v_norm, v_light_dir), 0);
  vec3 diffuse = v_diffuse_strength * light_colour;
  
  vec3 v_r = -v_light_dir + ((2 * dot(v_light_dir, v_norm)) * v_norm);
  vec3 v_eye_dir = -normalize(frag_view_pos);
  float v_specular_strength = pow(max(dot(v_r, v_eye_dir), 0.f), 64);
  vec3 specular = 1*v_specular_strength * light_colour;

  vec3 result = (specular + diffuse + ambient) * object_colour;

  frag_colour = vec4(result, 1.0);
}