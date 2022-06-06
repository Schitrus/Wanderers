#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 MVP;
uniform mat4 model;

out vec3 normal_frag;
out vec3 position_frag;
out vec3 model_position;

void main(){
    gl_Position = MVP * vec4(position, 1.0f);
    position_frag = vec3(model * vec4(position, 1.0f));
    normal_frag = mat3(transpose(inverse(model))) * normal;
    model_position = position;
}