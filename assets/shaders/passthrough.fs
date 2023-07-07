#version 460 core
out vec4 FragColor;
in vec2 tex_coords;
in vec3 normal;

uniform sampler2D current_texture;
void main()
{
    FragColor = vec4(texture(current_texture, tex_coords).rgb, 1.0);
}
