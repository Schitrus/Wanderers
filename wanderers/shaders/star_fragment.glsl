#version 460 core
out vec4 frag_color;

in vec3 geo_color;

uniform float time_point;

void main(){
    float color_size = 3.0f * pow((geo_color.x + geo_color.y + geo_color.z) / 3.0f, 2.0f);
    vec3 star_color = geo_color * (0.2f * sin(color_size * time_point + 10.0f * color_size) + 0.6f);
    frag_color = vec4(star_color, 1.0f);
} 