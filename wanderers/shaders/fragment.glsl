#version 460 core
layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 light_strength;

uniform vec3 color;
uniform vec3 light_position;
uniform vec3 camera_position;

uniform bool is_sun;

uniform sampler3D surface_texture;

in vec3 normal_frag;
in vec3 position_frag;
in vec3 model_position;
in vec3 texture_coord;

void main(){
    vec3 light_direction = normalize(light_position - position_frag);
    vec3 camera_direction = normalize(camera_position - position_frag);
    vec3 normal = normalize(normal_frag);
    vec3 reflect_direction = reflect(-camera_direction, normal);

    vec4 texture_color = texture(surface_texture, texture_coord);
    vec3 tex_color = is_sun ? vec3(1.0f) : vec3(texture_color);

    bool is_on_land = texture_color.a > 0.5f;

    float ambient = 0.01f;
    float diffuse = max(dot(normal, light_direction), 0.0f);
    float specular = diffuse * 0.5f * pow(max(dot(camera_direction, reflect_direction), 0.0f), 16.0f * (is_on_land ? 1.0f : 8.0f));



    vec3 terrain_color = (is_on_land && !is_sun) ? tex_color : color;

    vec3 object_color = is_sun ? terrain_color : terrain_color * (ambient + diffuse + specular);

    //vec3 up_color = vec3(1.0f, 0.0f, 0.0f);
    //vec3 down_color = vec3(0.0f, 0.0f, 1.0f);
    //vec3 front_color = vec3(0.0f, 1.0f, 0.0f);
    //
    //frag_color = vec4(mix(mix(mix(object_color, up_color, model_position.y), down_color, -model_position.y), front_color, model_position.x), 1.0f);



    frag_color = vec4(object_color, 1.0f);
    light_strength = vec4(is_sun ? 50.0f : 0.0f);
} 