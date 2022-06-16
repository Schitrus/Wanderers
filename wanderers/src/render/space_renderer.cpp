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
#include "glm/gtx/rotate_vector.hpp"

/* STL Includes */
#include <typeinfo>

#include <iostream>

namespace wanderers {
namespace render {

SpaceRenderer::SpaceRenderer(render::shader::ShaderProgram* shader, render::Camera* camera)
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
	
	camera_->lock();

	if (render_width_ != 0 && render_height_ != 0) {
		camera_->setAspectRatio(render_width_, render_height_);
	}
	

	view_ = camera_->getViewMatrix();
	projection_ = camera_->getProjectionMatrix();

	glViewport(0, 0, render_width_, render_height_);
	if (camera_->shouldClear()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	shader_->use();

	glEnable(GL_DEPTH_TEST);
}

/*
 * SpaceRenderer preRender:
 * - Disable OpenGL pipline operations.
 * - Swap frame for smooth transition.
 * - Poll all queued events.
 */
void SpaceRenderer::postRender() {
	camera_->unlock();

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
	preRender();

	for (simulation::object::Stars* stars : space_simulation->getGroupOfStars())
		render(stars);
	for (simulation::object::OrbitalSystem* solar_system : space_simulation->getSolarSystems())
		render(solar_system);

	postRender();
}

/*
 * SpaceRenderer render Stars:
 * - Render the solar system.
 */
void SpaceRenderer::render(simulation::object::Stars* stars) {
	simulation::object::AggregateObject* star_object{ stars->getPhysicalObject() };

	glm::vec3 camera_position{ camera_->getPosition() };
	glm::mat4 agg_model{ glm::translate(glm::mat4{1.0f}, camera_position * (1.0f - 1.0f / stars->getDistance())) /* * star_object->getMatrix()*/ };
	for (std::pair<simulation::object::Object*, glm::vec3> object : star_object->getObjects()) {
		object.first->bind();

		glm::mat4 obj_model{ agg_model /* * glm::translate(glm::mat4{1.0f}, object.second) * object.first->getMatrix()*/};
		shader_->setUniform(obj_model, "model");
		shader_->setUniform(projection_ * view_ * obj_model, "MVP");
		shader_->setUniform(glm::vec3(0.0f, 0.0f, 0.0f), "light_position");
		shader_->setUniform(camera_position, "camera_position");
		shader_->setUniform(stars->getColor() * 0.8f * static_cast<float>(0.25f * sin(stars->getSize() * glfwGetTime() + 10.0f * stars->getSize()) + 0.75f), "color");
		shader_->setUniform(true, "is_sun");

		glDisable(GL_DEPTH_TEST);
		glPointSize(stars->getSize() * (render_width_ / 2000.0f) * sqrt(60.0f / camera_->getFieldOfView()));
		glDrawArrays(GL_POINTS, 0, object.first->getModel()->size());
		glPointSize(1.0f);
		glEnable(GL_DEPTH_TEST);

		object.first->unbind();
	}
}

/*
 * SpaceRenderer render OrbitalSystem:
 * - Set the shaders light position.
 * - Set the shaders camera position.
 * - Render the orbitee.
 * - Render each orbit.
 */
void SpaceRenderer::render(simulation::object::OrbitalSystem* orbital_system, glm::mat4 transform) {
	shader_->setUniform(glm::vec3(0.0f, 0.0f, 0.0f), "light_position");
	shader_->setUniform(camera_->getPosition(), "camera_position");

	glm::mat4 system_matrix{ transform * orbital_system->getMatrix() };

	for (std::pair<simulation::object::AstronomicalObject*, simulation::object::Orbit*> orbit : orbital_system->getOrbits()) {
		render(orbit.first, system_matrix * orbit.second->getMatrix());
	}
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
	simulation::object::AggregateObject* solar_object{ solar->getPhysicalObject() };

	glm::mat4 agg_model{ transform * solar->getMatrix() * solar_object->getMatrix() };
	for (std::pair<simulation::object::Object*, glm::vec3> object : solar_object->getObjects()) {
		object.first->bind();

		glm::mat4 obj_model{ agg_model * glm::translate(glm::mat4{1.0f}, object.second) * object.first->getMatrix() };
		shader_->setUniform(obj_model, "model");
		shader_->setUniform(projection_ * view_ * obj_model, "MVP");
		shader_->setUniform(solar->getColor(), "color");
		shader_->setUniform(true, "is_sun");

		glDrawArrays(GL_TRIANGLES, 0, object.first->getModel()->size());
		object.first->unbind();
	}
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
	simulation::object::AggregateObject* planet_object{ planet->getPhysicalObject() };
	
	glm::mat4 agg_model{ transform * planet->getMatrix() * planet_object->getMatrix() };
	for (std::pair<simulation::object::Object*, glm::vec3> object : planet_object->getObjects()) {
		object.first->bind();

		glm::mat4 obj_model{ agg_model * glm::translate(glm::mat4{1.0f}, object.second) * object.first->getMatrix() };
		shader_->setUniform(obj_model, "model");
		shader_->setUniform(projection_ * view_ * obj_model, "MVP");
		shader_->setUniform(planet->getColor(), "color");
		shader_->setUniform(false, "is_sun");

		glDrawArrays(GL_TRIANGLES, 0, object.first->getModel()->size());
		object.first->unbind();
	}
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
