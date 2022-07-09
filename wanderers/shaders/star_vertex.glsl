#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 view;
uniform mat4 projection;

out VertOut{
    vec3 vcolor;
} vert_out;

void main(){
    vec4 view_pos = view * vec4(position, 1.0f);
    gl_Position = view_pos;
    vert_out.vcolor = color;
}