#version 460 core
out vec4 FragColor;
in vec3 normal;
in vec2 tex_coords;
in vec3 world_position;
in vec4 frag_position_light_space;


float ambient_str = 0.25;
float diffuse_str = 0.4;
float specular_str = 0.4;

uniform sampler2D current_texture;
uniform sampler2D shadow_map;

uniform vec3 light_position;
uniform vec3 view_position;

float get_shadow(vec4 frag_pos_ls)
{
    vec3 proj_coords = frag_position_light_space.xyz / frag_position_light_space.w;
    proj_coords = proj_coords * 0.5 + 0.5;
    if(proj_coords.z > 1.0)
    {
        return 0.0;
    }

    float bias = 0.005;
    float shadow = 0.0;
    float current_depth = proj_coords.z;
    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; x++)
    {
        for(int y = -1; y <= 1; y++)
        {
            float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r;
            shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;


    return shadow;
}
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
    float shadow = get_shadow(frag_position_light_space);

    FragColor = vec4(ambient + (1.0 - shadow) * (diffuse + specular), 1.0);
}
