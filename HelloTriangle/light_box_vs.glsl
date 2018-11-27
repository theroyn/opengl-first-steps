#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 in_normal;

out vec3 frag_normal;
out vec3 frag_model_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  frag_normal = in_normal;
  
  frag_model_pos = vec3(model * vec4(pos, 1.0));
  gl_Position = projection * view * model * vec4(frag_model_pos, 1.0);
}