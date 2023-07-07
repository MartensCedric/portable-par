#version 460 core
out vec4 FragColor;
in vec3 normal;
in vec2 tex_coords;
in vec3 world_position;

float ambient_str = 0.4;
float diffuse_str = 0.6;

uniform sampler2D current_texture;
uniform vec3 light_position;

void main()
{
    vec3 tex_color = texture(current_texture, tex_coords).rgb;
    vec3 light_direction = light_position - world_position;
    float light_str = max(0, dot(normalize(light_position), normal));
    vec3 ambient = ambient_str * tex_color;
    vec3 diffuse = diffuse_str * light_str * tex_color;
    FragColor = vec4(ambient + diffuse, 1.0);
}
