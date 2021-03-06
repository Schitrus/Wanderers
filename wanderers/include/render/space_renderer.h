/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class renders the space simulation.                                  *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_RENDER_SPACE_RENDERER_H_
#define WANDERERS_RENDER_SPACE_RENDERER_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes*/
#include "render/shader/shader_program.h"
#include "render/camera.h"

#include "simulation/space_simulation.h"
#include "simulation/object/orbital_system.h"
#include "simulation/object/orbit.h"
#include "simulation/object/solar.h"
#include "simulation/object/planet.h"
#include "simulation/object/astronomical_object.h"

namespace wanderers {
namespace render {

/*
 * Class to render the space simulation with the given shader program from the given camera.
 * It renders the whole simulation by going down the simulation structure and drawing each part.
 */
class SpaceRenderer {
public:
	SpaceRenderer(render::shader::ShaderProgram* shader, render::Camera* camera);

	/* Does some pre render operations. Has to be done befor each render iteration.*/
	void preRender();
	/* Does some post render operations. Has to be done after each render iteration. */
	void postRender();

	/* Renders the objects within the space simulation. */
	void render(simulation::SpaceSimulation* space_simulation);

	/* Renders all the stars of the sky. */
	void render(simulation::object::Stars* stars);

	/* Renders the objects of an orbital system. */
	void render(simulation::object::OrbitalSystem* orbital_system, glm::mat4 transform = glm::mat4{ 1.0f });

	/* Renders the orbit. */
	void render(simulation::object::Orbit* orbit, glm::vec3 color, glm::mat4 transform = glm::mat4{ 1.0f });

	/* Renders the solar object. */
	void render(simulation::object::Solar* solar, glm::mat4 transform = glm::mat4{ 1.0f });
	/* Renders the planet object. */
	void render(simulation::object::Planet* planet, glm::mat4 transform = glm::mat4{ 1.0f });
	/* Inteprets the type of astronomical object and renders it accordingly. */
	void render(simulation::object::AstronomicalObject* object, glm::mat4 transform = glm::mat4{ 1.0f });

	/* Functions to control if orbits should be rendered. */
	bool showOrbits();
	void setShowOrbits(bool show_orbits);
	bool getShowOrbits();

private:
	int render_width_{};
	int render_height_{};

	render::shader::ShaderProgram* shader_;

	render::Camera* camera_;

	glm::mat4 projection_;
	glm::mat4 view_;

	bool show_orbits_;
};

} // namespace render
} // namespace wanderers

#endif // WANDERERS_RENDER_SPACE_RENDERER_H_
