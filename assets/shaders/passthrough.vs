#version 460 core
layout (location = 0) in vec3 position;

void main()
{
    glPosition = vec4(position.xyz, 1.0)
}
