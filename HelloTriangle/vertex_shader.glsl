#version 400

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;

out vec3 colour;
uniform float light;

void main()
{
  gl_Position = vec4(pos, 1.0);
  //colour = col*light;
  colour = pos;
}