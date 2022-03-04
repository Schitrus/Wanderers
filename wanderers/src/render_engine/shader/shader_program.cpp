/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the ShaderProgram class.                                *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "render_engine/shader/shader_program.h"

/* External Includes */
#include "glad/gl.h"

/* STL Includes */
#include <iostream>

namespace render_engine {
namespace shader {

/*
 * ShaderProgram Constructor:
 * - Create the shaders.
 */
ShaderProgram::ShaderProgram(std::string vertex_path, std::string fragment_path) : program_id_{ 0 }, shaders_{}  {
	Shader vertex_shader{ GL_VERTEX_SHADER, vertex_path };
	Shader fragment_shader{ GL_FRAGMENT_SHADER, fragment_path };

	shaders_.push_back(vertex_shader);
	shaders_.push_back(fragment_shader);
}

/*
 * SahderProgram link:
 * - Read and compiles the shaders.
 * - Link the shader program.
 * - Return 0 if linking was succesful.
 */
int ShaderProgram::link() {
	program_id_ = glCreateProgram();

	for (Shader shader : shaders_) {
		shader.read();
		shader.compile();
		glAttachShader(program_id_, shader.getShaderID());
	}

	glLinkProgram(program_id_);

	int  success;
	char info_log[512];
	glGetProgramiv(program_id_, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program_id_, 512, nullptr, info_log);
		std::cout << "Error: Shader program linkage failed.\n" << info_log << std::endl;
	}
	return !success; // if the linking was successful return 0
}

void ShaderProgram::use() {
	glUseProgram(program_id_);
}

void ShaderProgram::setUniform(glm::mat4 matrix, const char* name) {
	glUniformMatrix4fv(glGetUniformLocation(program_id_, name), 1, false, &matrix[0][0]);
}

void ShaderProgram::setUniform(glm::vec3 vector, const char* name) {
	glUniform3f(glGetUniformLocation(program_id_, name), vector.x, vector.y, vector.z);
}

void ShaderProgram::setUniform(int integer, const char* name) {
	glUniform1i(glGetUniformLocation(program_id_, name), integer);
}

GLuint ShaderProgram::getProgramID() {
	return program_id_;
}

} // namespace shader
} // namespace render_engine