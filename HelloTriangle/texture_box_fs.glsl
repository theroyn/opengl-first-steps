#version 330 core

in vec2 pp_texture;

out vec4 fragment_colour;

uniform sampler2D texture_sampler1;
uniform sampler2D texture_sampler2;
uniform float vis;

void main()
{
  vec2 smiley_tex = vec2(1 - pp_texture.x, pp_texture.y);
  fragment_colour = mix(texture(texture_sampler1, pp_texture), texture(texture_sampler2, smiley_tex), vis);
}