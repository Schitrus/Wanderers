#version 460 core
layout (points) in;

layout (triangle_strip, max_vertices = 750) out;

uniform mat4 view;
uniform mat4 projection;
uniform float aspect_ratio;

in VertOut{
    vec3 vcolor;
} geo_in[];

out vec3 geo_color;

void circle(vec4 position){
    float PI = 3.1415f;
    float max_points = 25.0f;
    for(int i = 0; i < max_points; i++){
        gl_Position = projection * (view * position + vec4(sin(float(i) * 2.0f*PI / max_points), cos(float(i) * 2.0f*PI / max_points), 0.0f, 0.0f)); 
        EmitVertex();
        gl_Position =  projection * view * position; 
        EmitVertex();
        gl_Position = projection * (view * position + vec4(sin(float(i + 1) * 2.0f*PI / max_points), cos(float(i + 1) * 2.0f*PI / max_points), 0.0f, 0.0f)); 
        EmitVertex();

        EndPrimitive();
    }
}

void main(){
    geo_color = geo_in[0].vcolor;
    circle( gl_in[0].gl_Position);
} 