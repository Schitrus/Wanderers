/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class renders the space simulation.                                  *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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

namespace render_engine {

/*
 * Class to render the space simulation with the given shader program from the given camera.
 * It renders the whole simulation by going down the simulation structure and drawing each part.
 */
class SpaceRenderer {
public:
	SpaceRenderer(render_engine::shader::ShaderProgram& shader, render_engine::Camera& camera);

	/* Does some pre render operations. Has to be done befor each render iteration.*/
	void preRender();
	/* Does some post render operations. Has to be done after each render iteration. */
	void postRender();

	/* Renders the objects of a orbital system. */
	void render(OrbitalSystem* orbital_system, glm::mat4 transform = glm::mat4{ 1.0f });

	/* Renders the objects of an orbit. */
	void render(Orbit* orbit, glm::mat4 transform = glm::mat4{ 1.0f });
	/* Renders the solar object. */
	void render(Solar* solar, glm::mat4 transform = glm::mat4{ 1.0f });
	/* Renders the planet object. */
	void render(Planet* planet, glm::mat4 transform = glm::mat4{ 1.0f });
	/* Inteprets the type of astronomical object and renders it accordingly. */
	void render(AstronomicalObject* object, glm::mat4 transform = glm::mat4{ 1.0f });

private:
	int render_width_{};
	int render_height_{};

	render_engine::shader::ShaderProgram& shader_;

	render_engine::Camera& camera_;

	glm::mat4 projection_;
	glm::mat4 view_;
};

} // namespace render_engine

#endif // WANDERERS_RENDER_ENGINE_SPACE_RENDERER_H_
