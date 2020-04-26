#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texture_coords;

out vec2 frag_texture_coords;
out vec4 frag_particle_color;

uniform mat4 projection_matrix;
uniform vec2 offset;
uniform vec4 color;

void main()
{
    float scale = 10.0f;
    frag_texture_coords = texture_coords;

    frag_particle_color = color;

    gl_Position = projection_matrix * vec4((position * scale) + offset, 0.0, 1.0);
}
