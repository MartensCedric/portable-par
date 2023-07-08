#version 460 core
out vec4 FragColor;
in vec3 normal;
in vec2 tex_coords;
in vec3 world_position;


float ambient_str = 0.25;
float diffuse_str = 0.4;
float specular_str = 0.4;

uniform sampler2D current_texture;
uniform vec3 light_position;
uniform vec3 view_position;

void main()
{
    vec3 tex_color = texture(current_texture, tex_coords).rgb;
    vec3 light_direction = normalize(light_position - world_position);
    vec3 view_direction = normalize(view_position - world_position);
     vec3 reflect_direction = reflect(-light_direction, normal);

    float light_str = max(0, dot(light_direction, normal));
    vec3 ambient = ambient_str * tex_color;
    vec3 diffuse = diffuse_str * light_str * tex_color;

    float specular_value = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular = specular_str * specular_value * vec3(1.0, 1.0, 1.0);

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
