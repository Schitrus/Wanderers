#version 460 core
out vec4 frag_color;

uniform vec3 color;
uniform vec3 light_position;

uniform bool is_sun;

in vec3 normal_frag;
in vec3 position_frag;

void main(){
    vec3 light_direction = normalize(light_position - position_frag);
    vec3 normal = normalize(normal_frag);
    frag_color = is_sun ? vec4(color, 1.0f) : vec4(color * min(1.0f, max(0.0f, dot(normal, light_direction)) + 0.02f), 1.0f);
} 