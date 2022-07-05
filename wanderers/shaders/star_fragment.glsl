#version 460 core
layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 light_strength;

in vec3 geo_color;

in vec3 og_color;

in vec2 geo_uv;

uniform float time_point;

void main(){
    float color_size = 3.0f * pow((og_color.x + og_color.y + og_color.z) / 3.0f, 2.0f);
    float star_dynamic = (0.2f * sin(color_size * time_point + 10.0f * color_size) + 0.6f);
    vec3 star_color = geo_color;
    float spike = 1.0f/((abs((10.0f)*geo_uv.x) + 1)*(abs((10.0f)*geo_uv.y) + 1)) - (1.0f/(10.0f));
    spike /= star_dynamic;
    
    float color_normalizer = max(max(star_color.r, star_color.g), star_color.b);
    
    frag_color = vec4(star_color/color_normalizer, (1.0f - pow(gl_FragCoord.z, 10000.0f)) * spike);
    light_strength = vec4(5.0f * color_size * (0.1f * sin(color_size * time_point + 10.0f * color_size) + 0.9f));
} 