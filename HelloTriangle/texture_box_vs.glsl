#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex_uv;

out vec2 pp_texture;

void main()
{
  gl_Position = vec4(pos, 1.0);
  pp_texture = vec2(tex_uv.x, tex_uv.y);
}