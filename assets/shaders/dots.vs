#version 460 core
layout (location = 0) in vec3 _position;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view  * vec4(_position.xyz, 1.0);
}
