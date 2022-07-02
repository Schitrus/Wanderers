#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 view;
uniform mat4 projection;

out VertOut{
    vec3 vcolor;
} vert_out;

void main(){
    gl_Position = projection * view * vec4(position, 1.0f);
    vert_out.vcolor = color;
}