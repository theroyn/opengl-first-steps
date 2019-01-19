#version 400

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex;

// Model oriented
out vec3 frag_normal;
out vec3 frag_model_pos;

// View oriented
out vec3 frag_view_normal;
out vec4 frag_view_pos;
out vec2 frag_tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  frag_normal = in_normal;
  frag_tex = in_tex;
  frag_model_pos = (model * vec4(pos, 1.0)).xyz;

  mat3 normal_matrix = transpose(inverse(mat3(view * model)));
  frag_view_normal = normal_matrix * in_normal;
  frag_view_pos = view * model * vec4(pos, 1.0);
  gl_Position = projection * view * vec4(frag_model_pos, 1.0);
}