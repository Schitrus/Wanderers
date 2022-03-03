#ifndef WANDERERS_RENDER_ENGINE_SHADER_SHADER_PROGRAM_H_
#define WANDERERS_RENDER_ENGINE_SHADER_SHADER_PROGRAM_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes */
#include "render_engine/shader/shader.h"

/* STL Includes */
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

	unsigned int getProgramID();
private:
	unsigned int program_id_;

	std::vector<Shader> shaders_;
};

#endif // WANDERERS_RENDER_ENGINE_SHADER_SHADER_PROGRAM_H_
