#include "render_engine/shader/shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

int Shader::read() {
	std::ifstream shader_file;
	std::stringstream shader_content;
	try {
		shader_file.open(shader_path_);
		shader_content << shader_file.rdbuf();
		shader_file.close();
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
		return 1;
	}

	std::string shader_string{ shader_content.str() };
	shader_content_ = new char[shader_string.length() + 1];
	memcpy(shader_content_, shader_string.c_str(), shader_string.length() + 1);

	return 0;
}

int Shader::compile() {
	shader_id_ = glCreateShader(shader_type_);

	glShaderSource(shader_id_, 1, &shader_content_, nullptr);
	glCompileShader(shader_id_);

	int  success;
	char info_log[512];
	glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(shader_id_, 512, nullptr, info_log);
		std::cout << "Error: Shader compilation failed for "<< shader_path_ << ".\n" << info_log << std::endl;
	}
	return success;
}

const char* Shader::fileContent() {
	return shader_content_;
}

GLuint Shader::getShaderID() {
	return shader_id_;
}
