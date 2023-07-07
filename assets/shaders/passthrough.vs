#version 460 core
layout (location = 0) in vec3 _position;
layout (location = 1) in vec3 _normal;
layout (location = 2) in vec2 _tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 world_position;
out vec3 normal;
out vec2 tex_coords;

void main()
{
    gl_Position = projection * view * model * vec4(_position.xyz, 1.0);
    world_position =  vec3(model * vec4(_position.xyz, 1.0));
    normal = mat3(transpose(inverse(model))) * _normal;
    tex_coords = _tex_coords;
}
