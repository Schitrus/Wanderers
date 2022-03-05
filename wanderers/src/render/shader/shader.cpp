/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Shader class.                                       *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "render/shader/shader.h"

/* External Includes */
#include "glad/gl.h"

/* STL Includes */
#include <fstream>
#include <iostream>
#include <sstream>

namespace wanderers {
namespace render {
namespace shader {

Shader::Shader(unsigned int shader_type, std::string shader_path) 
	: shader_id_{0}, shader_type_{ shader_type }, shader_path_{ shader_path }, shader_content_{nullptr} {}

/*
 * Shader read:
 * - Read whole file if it exists.
 * - Store content of the file in a char pointer.
 * - Return 0 if succesful.
 */
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

/*
 * Shader compile:
 * - Create shader from the read file content.
 * - Compile shader.
 * - Return 0 if successful.
 */
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
	return !success; // if the compilation was successful return 0
}

const char* Shader::fileContent() {
	return shader_content_;
}

unsigned int Shader::getShaderID() {
	return shader_id_;
}

} // namespace shader
} // namespace render
} // namespace wanderers
