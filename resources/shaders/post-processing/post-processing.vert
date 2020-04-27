#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texture_coords;

out vec2 frag_texture_coords;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

void main()
{
    gl_Position = vec4(position, 0.0f, 1.0f);
    vec2 texture = texture_coords;
    if(chaos)
    {
        float strength = 0.3;
        vec2 pos = vec2(texture.x + sin(time) * strength, texture.y + cos(time) * strength);
        frag_texture_coords = pos;
    }
    else if(confuse)
    {
        frag_texture_coords = vec2(1.0 - texture.x, 1.0 - texture.y);
    }
    else
    {
        frag_texture_coords = texture_coords;
    }
    if (shake)
    {
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;
        gl_Position.y += cos(time * 15) * strength;
    }
}
