#version 400

// macros
#define LIGHTS_N 4
#define DEBUG 1

// debug declarations
#ifdef DEBUG
void d_split(in float val, in float a, in float b, out vec4 res);
void d_split(in vec3 val, in float a, in float b, out vec4 res);
void d_print(in vec3 val, out vec4 res);
void d_print(in float val, out vec4 res);

#define RED vec4(1, 0, 0, 1)
#define GRN vec4(0, 1, 0, 1)
#define BLU vec4(0, 0, 1, 1)

bool d_fail = false;
vec4 d_res = vec4(1, 1, 1, 1.);
#endif

// structs
struct Material
{
  vec3 ambient;
  sampler2D diffuse;
  sampler2D specular;
  int shininess;
};

struct Light
{
  vec4 view_pos;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  
  float constant_c;
  float linear_c;
  float quadratic_c;
};

// I/Os
in vec3 frag_view_normal;
in vec4 frag_view_pos;
in vec2 frag_tex;

out vec4 frag_colour;

// uniforms
uniform mat4 view;
uniform Material material;
uniform Light light;
uniform sampler2D emission_map;
uniform float time;

uniform Light lights[LIGHTS_N];

// definitions
void calculate_dir_light(in int idx,
                         in vec3 material_ambient_diffuse,
                         in vec3 material_specular,
                         out vec3 res)
{
  Light light = lights[idx];

  vec3 ambient = material_ambient_diffuse * light.ambient;

  vec3 light_dir = normalize(light.view_pos.xyz);

  vec3 norm = normalize(frag_view_normal);
  float diffuse_strength = max(dot(norm, light_dir), 0);
  vec3 diffuse = material_ambient_diffuse * diffuse_strength * light.diffuse;
  
  vec3 r = -light_dir + ((2 * dot(light_dir, norm)) * norm);
  vec3 eye_dir = -normalize(frag_view_pos.xyz);
  float spec = pow(max(dot(r, eye_dir), 0.f), material.shininess);
  vec3 specular = material_specular * spec * light.specular;

  res = specular + diffuse + ambient;
}

void calculate_pointy_light(in int idx,
                            in vec3 material_ambient_diffuse,
                            in vec3 material_specular,
                            out vec3 res)
{
  Light light = lights[idx];

  // ambient
  vec3 ambient = material_ambient_diffuse * light.ambient;
  
  // attenuation
  vec3 diff = (light.view_pos - frag_view_pos).xyz;

  vec3 light_dir = normalize(diff);
  float dist = length(diff);

  float attenuation = 1.f / (light.constant_c +
                             dist * light.linear_c +
                             dist * dist * light.quadratic_c); 

  // diffuse
  vec3 norm = normalize(frag_view_normal);

  float diffuse_strength = max(dot(norm, light_dir), 0);

  vec3 diffuse = material_ambient_diffuse * diffuse_strength * light.diffuse;
  
  // specular
  vec3 r = -light_dir + ((2 * dot(light_dir, norm)) * norm);
  vec3 eye_dir = -normalize(frag_view_pos.xyz);
  float spec = pow(max(dot(r, eye_dir), 0.f), material.shininess);
  vec3 specular = material_specular * spec * light.specular;

  res = (specular + diffuse + ambient) * attenuation ;
}

// main
void main()
{
  vec3 material_ambient_diffuse = texture(material.diffuse, frag_tex).rgb;
  vec3 material_specular = vec3(texture(material.specular, frag_tex));

  vec3 res_light = vec3(0.);

  for (int i = 0; i < LIGHTS_N; ++i)
  {
    vec3 single_light;

    if (lights[i].view_pos.w == 0.0)
    {
      calculate_dir_light(i, material_ambient_diffuse, material_specular, single_light);
    }
    else
    {
      calculate_pointy_light(i, material_ambient_diffuse, material_specular, single_light);
    }

    res_light += single_light;
  }

  // emission
  vec3 emission = vec3(0.0);
  if (material_specular.r ==  0.0)   /*rough check for blackbox inside spec texture */
  {        
      emission = texture(emission_map, frag_tex + vec2(0.0,time)).rgb;
  }

  frag_colour = vec4(res_light /**+ emission*/, 1.0);

#ifdef DEBUG
  if (d_fail) frag_colour = d_res;
#endif
}

/**================================== END OF MAIN ============================*/

// debug definitions
#ifdef DEBUG

void d_split(in float val, in float a, in float b, out vec4 res)
{
#ifdef DEBUG
  d_fail = true;

  if (val < a)
    res = RED;
  else if (val < b)
    res = GRN;
  else
    res = BLU;
#endif
}

void d_split(in vec3 val, in float a, in float b, out vec4 res)
{
  float n_val = (val.x + val.y + val.z) / 3.f;
  d_split(n_val, a, b, res);
}

void d_print(in vec3 val, out vec4 res)
{
#ifdef DEBUG
  d_fail = true;
  float gray = 0.21 * val.r + 0.72 * val.g + 0.07 * val.b;
  res = gray * vec4(1.);
#endif
}

void d_print(in float val, out vec4 res)
{
#ifdef DEBUG
  d_fail = true;
  res = vec4(val);
#endif
}

#endif // DEBUG
