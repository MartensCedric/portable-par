#version 460 core

layout (location = 0) in vec3 _position;
layout (location = 1) in vec2 _tex_coords;

out vec2 tex_coords;

void main()
{
    tex_coords = _tex_coords;
    gl_Position = vec4(_position, 1.0);
}