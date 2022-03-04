#ifndef WANDERERS_RENDER_ENGINE_SPACE_RENDERER_H_
#define WANDERERS_RENDER_ENGINE_SPACE_RENDERER_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes*/
#include "render_engine/shader/shader_program.h"
#include "render_engine/camera.h"

#include "simulation/object/orbital_system.h"
#include "simulation/object/orbit.h"
#include "simulation/object/solar.h"
#include "simulation/object/planet.h"
#include "simulation/object/astronomical_object.h"


class SpaceRenderer {
public:
	SpaceRenderer(render_engine::shader::ShaderProgram& shader, Camera& camera);

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

	render_engine::shader::ShaderProgram& shader_;

	Camera& camera_;

	glm::mat4 projection_;
	glm::mat4 view_;
};

#endif // WANDERERS_RENDER_ENGINE_SPACE_RENDERER_H_
