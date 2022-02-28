#include "SpaceRenderer.h" 

#include "glm/ext.hpp"

#include <iostream>

SpaceRenderer::SpaceRenderer(ShaderProgram& shader, Camera& camera)
	: shader_{ shader }, camera_{ camera }, view_{}, projection_{} { }

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

void SpaceRenderer::postRender() {
	glDisable(GL_DEPTH_TEST);

	glfwSwapBuffers(glfwGetCurrentContext());
	glfwPollEvents();
}

void SpaceRenderer::render(OrbitalSystem* orbital_system, glm::mat4 transform) {
	shader_.setUniform(glm::vec3(0.0f, 0.0f, 0.0f), "light_position");

	render(orbital_system->orbitee_, transform);
	for (Orbit* orbit : orbital_system->orbits_)
		render(orbit, transform);
}

void SpaceRenderer::render(Orbit* orbit, glm::mat4 transform) {
	glm::mat4 orbit_matrix{ orbit->getOrbitMatrix() };
	render(orbit->orbitor_, transform * orbit_matrix);
}

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

void SpaceRenderer::render(AstronomicalObject* object, glm::mat4 transform) {
	const std::type_info& object_type{ typeid(*object) };
	if (object_type == typeid(Solar))
		render(dynamic_cast<Solar*>(object), transform);
	else if (object_type == typeid(Planet))
		render(dynamic_cast<Planet*>(object), transform);
	else if (object_type == typeid(OrbitalSystem))
		render(dynamic_cast<OrbitalSystem*>(object), transform);
}