#version 330 core

in vec2 frag_texture_coords;

out vec4 color;

uniform sampler2D image;
uniform vec3 sprite_color;

void main()
{
    color = vec4(sprite_color, 1.0) * texture(image, frag_texture_coords);
}
