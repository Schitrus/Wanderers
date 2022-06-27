/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class links shaders and makes a shader program used by the GPU.      *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_RENDER_SHADER_SHADER_PROGRAM_H_
#define WANDERERS_RENDER_SHADER_SHADER_PROGRAM_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes */
#include "render/shader/shader.h"

/* STL Includes */
#include <string>
#include <vector>

namespace wanderers {
namespace render {
namespace shader {

// TODO: Generalize to different shader program types.

/*
 * Class for which to link and make shader program with shaders.
 * Also used to set uniform variables in the shaders.
 * NOTE: Only supports vertex-fragment shader for the time being.
 */
class ShaderProgram{
public:
	ShaderProgram(std::string vertex_path, std::string fragment_path);
	ShaderProgram(std::string vertex_path, std::string geometry_path, std::string fragment_path);

	/* Links shaders and creates */
	int link();

	// Use this Shader Program for graphical execution.
	void use();

	/* Sets uniform variables in the shaders of the Shader Program. */
	void setUniform(glm::mat4 matrix, const char* name);
	void setUniform(glm::vec3 vector, const char* name);
	void setUniform(int integer, const char* name);
	void setUniform(float decimal, const char* name);

	unsigned int getProgramID();
private:
	/* Unique shader program identification whitin the OpenGL context. */
	unsigned int program_id_;

	std::vector<Shader> shaders_;
};

} // namespace shader
} // namespace render
} // namespace wanderers

#endif // WANDERERS_RENDER_SHADER_SHADER_PROGRAM_H_
