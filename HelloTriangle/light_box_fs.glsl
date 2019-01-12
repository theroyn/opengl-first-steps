#version 400

//#define DEBUG 1
#define LIGHTS_N 1

struct Material
{
  vec3 ambient;
  sampler2D diffuse;
  sampler2D specular;
  int shininess;
};

struct Light
{
  vec4 pos;

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

//uniform vec4 light_view_vec;

uniform Material material;
uniform Light light;
uniform sampler2D emission_map;
uniform float time;
#ifdef DEBUG

#define RED vec4(1, 0, 0, 1)
#define GRN vec4(0, 1, 0, 1)
#define BLU vec4(0, 0, 1, 1)

bool d_fail = false;
vec4 d_res = vec4(1, 1, 1, 1.);

void d_split(in float val, in float a, in float b, out vec4 res)
{
  d_fail = true;

  if (val < a)
    res = RED;
  else if (val < b)
    res = GRN;
  else
    res = BLU;
}

void d_split(in vec3 val, in float a, in float b, out vec4 res)
{
  float n_val = (val.x + val.y + val.z) / 3.f;
  d_split(n_val, a, b, res);
}

#endif

uniform Light lights[LIGHTS_N];

void calculate_pointy_light(in int idx,
                            in vec3 material_ambient_diffuse,
                            in vec3 material_specular,
                            out vec3 res)
{
  //vec3 ambient = vec3(0.002);
  Light light = lights[idx];
#ifdef DEBUG
  if (idx == 0)
  {
    d_split(light.pos.w, -0.5, 0.5, d_res);
  }
#endif
  vec3 ambient = material_ambient_diffuse * light.ambient;

  vec3 light_dir = vec3(0.0);
  float dist = 1.f;

  if (light.pos.w == 0.0)
  {
    light_dir = light.pos.xyz;
  }
  else
  {
    vec3 diff = (light.pos - frag_view_pos).xyz;

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

  res = (specular + diffuse + ambient/**+ emission*/);// * attenuation ;
}

void main()
{
  vec3 material_ambient_diffuse = texture(material.diffuse, frag_tex).rgb;
  vec3 material_specular = vec3(texture(material.specular, frag_tex));

  vec3 pointy_light = vec3(0.);
  vec3 single_light;

  for (int i = 0; i < LIGHTS_N; ++i)
  {
    calculate_pointy_light(i, material_ambient_diffuse, material_specular, single_light);
    pointy_light += single_light;
  }
  frag_colour = vec4(pointy_light, 1.0);
  #ifdef DEBUG
  if (d_fail) frag_colour = d_res;
  #endif







//  vec3 ambient = material_ambient_diffuse * light.ambient;
//
//  vec3 light_dir = vec3(0.0);
//  float dist = 1.f;
//
//  if (light_view_vec.w == 0.0)
//  {
//    light_dir = light_view_vec.xyz;
//  }
//  else
//  {
//    vec3 diff = (light_view_vec - frag_view_pos).xyz;
//
//    light_dir = normalize(diff);
//    dist = length(diff);
//  }
//
//  float attenuation = 1.f / (light.constant_c + dist * light.linear_c + dist * dist * light.quadratic_c); 
//
//  vec3 norm = normalize(frag_view_normal);
//  float diffuse_strength = max(dot(norm, light_dir), 0);
//  vec3 diffuse = material_ambient_diffuse * diffuse_strength * light.diffuse;
//  
//  vec3 r = -light_dir + ((2 * dot(light_dir, norm)) * norm);
//  vec3 eye_dir = -normalize(frag_view_pos.xyz);
//  float spec = pow(max(dot(r, eye_dir), 0.f), material.shininess);
//  vec3 specular = material_specular * spec * light.specular;
//
//  /*Emission */
//    vec3 emission = vec3(0.0);
//    if (material_specular.r ==  0.0)   /*rough check for blackbox inside spec texture */
//    {
//        /*apply emission texture */
//        emission = texture(emission_map, frag_tex).rgb;
//        
//        /*some extra fun stuff with "time uniform" */
//        emission = texture(emission_map, frag_tex + vec2(0.0,time)).rgb;
//       // emission = emission * (sin(time) * 0.5 + 0.5) * 2.0;
//    }
//
//  vec3 result = (specular + diffuse + ambient+ emission) * attenuation ;

}