/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class reads in a shader file and compiles it.                        *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_RENDER_SHADER_SHADER_H_
#define WANDERERS_RENDER_SHADER_SHADER_H_

/* STL Includes */
#include <string>

namespace wanderers {
namespace render {
namespace shader {

/*
 * Class for which to read and compile shader.
 */
class Shader{
public:
	Shader(unsigned int shader_type, std::string shader_path);

	/* Reads whole shader file and stores it. */
	int read();

	/* Compiles the stored shader file. Must be read first. */
	int compile();

	/* Returns the shader file content. */
	const char* fileContent();

	unsigned int getShaderID();
private:
	/* Unique shader identification whitin the OpenGL context. */
	unsigned int shader_id_;

	const unsigned int shader_type_;

	std::string shader_path_;

	char* shader_content_;
};

} // namespace shader
} // namespace render
} // namespace wanderers

#endif // WANDERERS_RENDER_SHADER_SHADER_H_

