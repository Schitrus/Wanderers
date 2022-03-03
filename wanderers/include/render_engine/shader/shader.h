#ifndef WANDERERS_RENDER_ENGINE_SHADER_SHADER_H_
#define WANDERERS_RENDER_ENGINE_SHADER_SHADER_H_

/* STL Includes */
#include <string>

class Shader{
public:
	Shader(unsigned int shader_type, std::string shader_path);

	int read();

	int compile();

	const char* fileContent();

	unsigned int getShaderID();

private:
	unsigned int shader_id_;

	const unsigned int shader_type_;

	std::string shader_path_;

	char* shader_content_;
};
#endif // WANDERERS_RENDER_ENGINE_SHADER_SHADER_H_
