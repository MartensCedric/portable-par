#version 460 core

out vec4 FragColor;

in vec2 tex_coords;

uniform sampler2D depth_map;

uniform float near_plane;
uniform float far_plane;


void main()
{
    float z = texture(depth_map, tex_coords).r * 2.0 - 1.0;
    float depth_val = (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));

    FragColor = vec4(vec3(depth_val), 1.0);
}