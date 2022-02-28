#include "ShaderProgram.h"

#include <iostream>

ShaderProgram::ShaderProgram(std::string vertex_path, std::string fragment_path) {
	Shader vertex_shader{ GL_VERTEX_SHADER, vertex_path };
	Shader fragment_shader{ GL_FRAGMENT_SHADER, fragment_path };

	shaders_.push_back(vertex_shader);
	shaders_.push_back(fragment_shader);
}

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
	return success;
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