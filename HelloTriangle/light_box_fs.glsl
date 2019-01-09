#version 400

#define LIGHT_SOURCES_N 4

struct Material
{
  vec3 ambient;
  sampler2D diffuse;
  sampler2D specular;
  int shininess;
};

struct Light
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  
  float constant_c;
  float linear_c;
  float quadratic_c;
};

// View oriented
in vec3 frag_view_normal;
in vec4 frag_view_pos;
in vec2 frag_tex;

out vec4 frag_colour;

uniform vec4 light_view_vec;

uniform Material material;
uniform Light lights[LIGHT_SOURCES_N];
uniform sampler2D emission_map;
uniform float time;

void calculate_dir_light(Light light);
vec3 calculate_point_light(Light light)
{
  vec3 material_ambient_diffuse = texture(material.diffuse, frag_tex).rgb;
  vec3 material_specular = vec3(texture(material.specular, frag_tex));

  vec3 ambient = material_ambient_diffuse * light.ambient;

  vec3 light_dir = vec3(0.0);
  float dist = 1.f;

  if (light_view_vec.w == 0.0)
  {
    light_dir = light_view_vec.xyz;
  }
  else
  {
    vec3 diff = (light_view_vec - frag_view_pos).xyz;

    light_dir = normalize(diff);
    dist = length(diff);
  }

  float attenuation = 1.f / (light.constant_c + dist * light.linear_c + dist * dist * light.quadratic_c); 

  vec3 norm = normalize(frag_view_normal);
  float diffuse_strength = max(dot(norm, light_dir), 0);
  vec3 diffuse = material_ambient_diffuse * diffuse_strength * light.diffuse;
  
  vec3 r = -light_dir + ((2 * dot(light_dir, norm)) * norm);
  vec3 eye_dir = -normalize(frag_view_pos.xyz);
  float spec = pow(max(dot(r, eye_dir), 0.f), material.shininess);
  vec3 specular = material_specular * spec * light.specular;

  vec3 result = (specular + diffuse + ambient) * attenuation;

  return result;
}

void main()
{


  /*Emission */
    vec3 emission = vec3(0.0);
    if (material_specular.r ==  0.0)   /*rough check for blackbox inside spec texture */
    {
        /*apply emission texture */
        emission = texture(emission_map, frag_tex).rgb;
        
        /*some extra fun stuff with "time uniform" */
        emission = texture(emission_map, frag_tex + vec2(0.0,time)).rgb;
       // emission = emission * (sin(time) * 0.5 + 0.5) * 2.0;
    }

  vec3 result = (specular + diffuse + ambient) * attenuation; //+ emission;

  frag_colour = vec4(result, 1.0);
}