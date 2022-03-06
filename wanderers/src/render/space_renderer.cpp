/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the SpaceRender class.                                  *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "render/space_renderer.h" 

/* External Includes */
#include "glad/gl.h"
#include "glfw/glfw3.h"
#include "glm/ext.hpp"

/* STL Includes */
#include <typeinfo>

namespace wanderers {
namespace render {

SpaceRenderer::SpaceRenderer(render::shader::ShaderProgram& shader, render::Camera& camera)
	: shader_{ shader }, camera_{ camera }, view_{}, projection_{} { }

/*
 * SpaceRenderer preRender:
 * - Adapt the camera to the window size.
 * - Get the view and projection matrix.
 * - Set the viewport.
 * - Clear the screen.
 * - Use the shader.
 * - Enable OpenGL pipline operations.
 */
void SpaceRenderer::preRender() {
	
	
	glfwGetFramebufferSize(glfwGetCurrentContext(), &render_width_, &render_height_);
	camera_.setAspectRatio(render_width_, render_height_);

	camera_.lock();

	view_ = camera_.getViewMatrix();
	projection_ = camera_.getProjectionMatrix();

	glViewport(0, 0, render_width_, render_height_);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader_.use();

	glEnable(GL_DEPTH_TEST);
}

/*
 * SpaceRenderer preRender:
 * - Disable OpenGL pipline operations.
 * - Swap frame for smooth transition.
 * - Poll all queued events.
 */
void SpaceRenderer::postRender() {
	camera_.unlock();
	glDisable(GL_DEPTH_TEST);

	glfwSwapBuffers(glfwGetCurrentContext());
	glfwPollEvents();
}

/*
 * SpaceRenderer render SpaceSimulation:
 * - Render the solar system.
 * - Render the stars.
 */
void SpaceRenderer::render(simulation::SpaceSimulation* space_simulation) {
	for (simulation::object::Stars* stars : space_simulation->getGroupOfStars())
		render(stars);
	for (simulation::object::OrbitalSystem* solar_system : space_simulation->getSolarSystems())
		render(solar_system);
}

/*
 * SpaceRenderer render Stars:
 * - Render the solar system.
 */
void SpaceRenderer::render(simulation::object::Stars* stars) {
	stars->bind();

	glm::vec3 camera_position{ camera_.getPosition() };
	glm::mat4 model{ glm::translate(glm::mat4{1.0f}, camera_position) };
	shader_.setUniform(glm::vec3(0.0f, 0.0f, 0.0f), "light_position");
	shader_.setUniform(camera_position, "camera_position");
	shader_.setUniform(model, "model");
	shader_.setUniform(projection_ * view_ * model, "MVP");
	shader_.setUniform(stars->getColor() * static_cast<float>(0.25f*sin(stars->getSize() * glfwGetTime() + 10.0f * stars->getSize()) + 0.75f), "color");
	shader_.setUniform(true, "is_sun");

	glDisable(GL_DEPTH_TEST);
	glPointSize(stars->getSize());
	glDrawArrays(GL_POINTS, 0, stars->getSurface()->size());
	glPointSize(1.0f);
	glEnable(GL_DEPTH_TEST);

	stars->unbind();
}

/*
 * SpaceRenderer render OrbitalSystem:
 * - Set the shaders light position.
 * - Set the shaders camera position.
 * - Render the orbitee.
 * - Render each orbit.
 */
void SpaceRenderer::render(simulation::object::OrbitalSystem* orbital_system, glm::mat4 transform) {
	shader_.setUniform(glm::vec3(0.0f, 0.0f, 0.0f), "light_position");
	shader_.setUniform(camera_.getPosition(), "camera_position");

	render(orbital_system->getOrbitee(), transform);
	for (simulation::object::Orbit* orbit : orbital_system->getOrbits())
		render(orbit, transform);
}

/*
 * SpaceRenderer render Orbit:
 * - Render the orbitor with the orbit transformation matrix.
 */
void SpaceRenderer::render(simulation::object::Orbit* orbit, glm::mat4 transform) {
	glm::mat4 orbit_matrix{ orbit->getOrbitMatrix() };
	render(orbit->getOrbitor(), transform * orbit_matrix);
}

/*
 * SpaceRenderer render Solar:
 * - Bind the solar model.
 * - Calculate the solar transformation matrix.
 * - Set uniforms specific to the Solar.
 * - Draw.
 * - Unbind the solar model.
 */
void SpaceRenderer::render(simulation::object::Solar* solar, glm::mat4 transform) {
	solar->bind();

	glm::mat4 solar_matrix{ solar->getSolarMatrix() };
	glm::mat4 model{ transform * solar_matrix };
	shader_.setUniform(model, "model");
	shader_.setUniform(projection_ * view_ * model, "MVP");
	shader_.setUniform(solar->getColor(), "color");
	shader_.setUniform(true, "is_sun");

	glDrawArrays(GL_TRIANGLES, 0, solar->getSurface()->size());
	solar->unbind();
}

/*
 * SpaceRenderer render Planet:
 * - Bind the planet model.
 * - Calculate the planet transformation matrix.
 * - Set uniforms specific to the Planet.
 * - Draw.
 * - Unbind the planet model.
 */
void SpaceRenderer::render(simulation::object::Planet* planet, glm::mat4 transform) {
	planet->bind();

	glm::mat4 planet_matrix{ planet->getPlanetMatrix() };
	glm::mat4 model{ transform * planet_matrix };
	shader_.setUniform(model, "model");
	shader_.setUniform(projection_ * view_ * model, "MVP");
	shader_.setUniform(planet->getColor(), "color");
	shader_.setUniform(false, "is_sun");

	glDrawArrays(GL_TRIANGLES, 0, planet->getSurface()->size());
	planet->unbind();
}

/*
 * SpaceRenderer render AstronomicalObject:
 * - Get the type of the object.
 * - Render the object depending on the type.
 * - Do nothing if the type is not renderable.
 */
void SpaceRenderer::render(simulation::object::AstronomicalObject* object, glm::mat4 transform) {
	const std::type_info& object_type{ typeid(*object) };
	if (object_type == typeid(simulation::object::Solar))
		render(dynamic_cast<simulation::object::Solar*>(object), transform);
	else if (object_type == typeid(simulation::object::Planet))
		render(dynamic_cast<simulation::object::Planet*>(object), transform);
	else if (object_type == typeid(simulation::object::OrbitalSystem))
		render(dynamic_cast<simulation::object::OrbitalSystem*>(object), transform);
}

} // namespace render
} // namespace wanderers
