#ifndef WANDERERS_RENDER_ENGINE_SHADER_SHADER_H_
#define WANDERERS_RENDER_ENGINE_SHADER_SHADER_H_

#include "glad/gl.h"

#include <string>

class Shader{
public:
	Shader(GLuint shader_type, std::string shader_path) : shader_type_{shader_type}, shader_path_ { shader_path } {}

	~Shader() {}

	int read();

	int compile();

	const char* fileContent();

	GLuint getShaderID();

private:
	GLuint shader_id_;

	const GLuint shader_type_;

	std::string shader_path_;

	char* shader_content_;
};
#endif // WANDERERS_RENDER_ENGINE_SHADER_SHADER_H_
