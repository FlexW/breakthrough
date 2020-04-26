#version 330 core

in vec2 frag_texture_coords;
in vec4 frag_particle_color;

out vec4 color;

uniform sampler2D sprite;

void main()
{
  // color = (texture(sprite, frag_texture_coords) * frag_particle_color);
  color = texture(sprite, frag_texture_coords) * frag_particle_color;
}
