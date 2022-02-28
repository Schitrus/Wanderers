#ifndef SPACE_RENDERER_H
#define SPACE_RENDERER_H

#include "glad/gl.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"

#include "ShaderProgram.h"

#include "OrbitalSystem.h"
#include "Orbit.h"
#include "Solar.h"
#include "Planet.h"
#include "AstronomicalObject.h"

#include "Camera.h"

class SpaceRenderer {
public:
	SpaceRenderer(ShaderProgram& shader, Camera& camera);

	void preRender();
	void postRender();

	void render(OrbitalSystem* orbital_system, glm::mat4 transform = glm::mat4{ 1.0f });

	void render(Orbit* orbit, glm::mat4 transform = glm::mat4{ 1.0f });
	void render(Solar* solar, glm::mat4 transform = glm::mat4{ 1.0f });
	void render(Planet* planet, glm::mat4 transform = glm::mat4{ 1.0f });
	void render(AstronomicalObject* object, glm::mat4 transform = glm::mat4{ 1.0f });

private:
	int render_width_{};
	int render_height_{};

	ShaderProgram& shader_;

	Camera& camera_;

	glm::mat4 projection_;
	glm::mat4 view_;
};

#endif // SPACE_RENDERER_H
