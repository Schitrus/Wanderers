/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Space Render class.                                 *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "render_engine/space_renderer.h" 

/* External Includes */
#include "glad/gl.h"
#include "glfw/glfw3.h"
#include "glm/ext.hpp"

/* STL Includes */
#include <iostream>

namespace render_engine {

SpaceRenderer::SpaceRenderer(render_engine::shader::ShaderProgram& shader, render_engine::Camera& camera)
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
	glDisable(GL_DEPTH_TEST);

	glfwSwapBuffers(glfwGetCurrentContext());
	glfwPollEvents();
}

/*
 * SpaceRenderer render OrbitalSystem:
 * - Set the shaders light position.
 * - Render the orbitee.
 * - Render each orbit.
 */
void SpaceRenderer::render(OrbitalSystem* orbital_system, glm::mat4 transform) {
	shader_.setUniform(glm::vec3(0.0f, 0.0f, 0.0f), "light_position");

	render(orbital_system->getOrbitee(), transform);
	for (Orbit* orbit : orbital_system->getOrbits())
		render(orbit, transform);
}

/*
 * SpaceRenderer render Orbit:
 * - Render the orbitor with the orbit transformation matrix.
 */
void SpaceRenderer::render(Orbit* orbit, glm::mat4 transform) {
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
void SpaceRenderer::render(Solar* solar, glm::mat4 transform) {
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
void SpaceRenderer::render(Planet* planet, glm::mat4 transform) {
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
void SpaceRenderer::render(AstronomicalObject* object, glm::mat4 transform) {
	const std::type_info& object_type{ typeid(*object) };
	if (object_type == typeid(Solar))
		render(dynamic_cast<Solar*>(object), transform);
	else if (object_type == typeid(Planet))
		render(dynamic_cast<Planet*>(object), transform);
	else if (object_type == typeid(OrbitalSystem))
		render(dynamic_cast<OrbitalSystem*>(object), transform);
}

} // namespace render_engine