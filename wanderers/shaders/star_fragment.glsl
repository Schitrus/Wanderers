#version 460 core
layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 light_strength;

in vec3 geo_color;

in vec3 og_color;

uniform float time_point;

void main(){
    float color_size = 3.0f * pow((og_color.x + og_color.y + og_color.z) / 3.0f, 2.0f);
    vec3 star_color = geo_color * (0.2f * sin(color_size * time_point + 10.0f * color_size) + 0.6f);
    vec3 depth_color = mix(star_color, vec3(0.0f, 0.0f, 0.0f), pow(gl_FragCoord.z, 10000.0f));
    float color_normalizer = max(max(max(depth_color.x, depth_color.y), depth_color.z), 1.0f);
    frag_color = vec4(depth_color / color_normalizer, 1.0f);
    light_strength = vec4(5.0f * color_size * (0.1f * sin(color_size * time_point + 10.0f * color_size) + 0.9f));
} 