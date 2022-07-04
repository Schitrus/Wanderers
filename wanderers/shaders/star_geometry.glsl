#version 460 core
layout (points) in;

layout (triangle_strip, max_vertices = 12) out;

uniform float aspect_ratio;
uniform float star_size;

in VertOut{
    vec3 vcolor;
} geo_in[];

out vec3 geo_color;

out vec2 geo_uv;

out vec3 og_color;

void circle(vec4 position){
    float PI = 3.1415f;
    float max_points = 4.0f;
    for(int i = 0; i < max_points; i++){
        vec2 p1 = star_size * 0.01f * position.z * vec2(sin(float(i) * 2.0f*PI / max_points) / aspect_ratio, cos(float(i) * 2.0f*PI / max_points));
        vec2 p2 = star_size * 0.01f * position.z * vec2(sin(float(i + 1) * 2.0f*PI / max_points) / aspect_ratio, cos(float(i + 1) * 2.0f*PI / max_points));
        gl_Position = position + vec4(p1, 0.0f, 0.0f); 
        geo_color = geo_in[0].vcolor;
        geo_uv = vec2(sin(float(i) * 2.0f*PI / max_points), cos(float(i) * 2.0f*PI / max_points));
        EmitVertex();
        gl_Position =  position; 
        geo_color = geo_in[0].vcolor * 2.0f;
        geo_uv = vec2(0, 0);
        EmitVertex();
        gl_Position = position + vec4(p2, 0.0f, 0.0f);
        geo_color = geo_in[0].vcolor;
        geo_uv = vec2(sin(float(i + 1) * 2.0f*PI / max_points), cos(float(i + 1) * 2.0f*PI / max_points));
        EmitVertex();

        EndPrimitive();
    }
}

void point(vec4 position){
    gl_Position = position; 

    EmitVertex();
    EndPrimitive();
}

void main(){
    og_color = geo_in[0].vcolor;
    circle( gl_in[0].gl_Position );
} 