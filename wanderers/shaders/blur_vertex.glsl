#version 460 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 coordinate;

out vec2 texture_coordinate;

void main(){
	texture_coordinate = coordinate;
	gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
}