#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "glad/gl.h"
#include "glm/glm.hpp"

#include "Shader.h"

#include <string>
#include <vector>

class ShaderProgram{
public:
	ShaderProgram(std::string vertex_path, std::string fragment_path);

	int link();

	void use();

	void setUniform(glm::mat4 matrix, const char* name);
	void setUniform(glm::vec3 vector, const char* name);
	void setUniform(int integer, const char* name);

	GLuint getProgramID();
private:
	GLuint program_id_;

	std::vector<Shader> shaders_;
};

#endif // SHADER_PROGRAM_H
