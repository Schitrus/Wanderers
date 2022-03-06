#version 460 core
out vec4 frag_color;

uniform vec3 color;
uniform vec3 light_position;
uniform vec3 camera_position;

uniform bool is_sun;

in vec3 normal_frag;
in vec3 position_frag;

void main(){
    vec3 light_direction = normalize(light_position - position_frag);
    vec3 camera_direction = normalize(camera_position - position_frag);
    vec3 normal = normalize(normal_frag);
    vec3 reflect_direction = reflect(-camera_direction, normal);

    float ambient = 0.00f;
    float diffuse = max(dot(normal, light_direction), 0.0f);
    float specular = diffuse * 0.5f * pow(max(dot(camera_direction, reflect_direction), 0.0f), 16);

    frag_color = is_sun ? vec4(color, 1.0f) : vec4(color * (ambient + diffuse + specular), 1.0f);
} 