#version 460 core
layout (location = 0) in vec3 _position;
layout (location = 1) in vec2 _tex_coords;

out vec2 tex_coords;

void main()
{
    gl_Position = vec4(_position.xyz, 1.0);
    tex_coords = _tex_coords;
}
