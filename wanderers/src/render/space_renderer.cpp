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

SpaceRenderer::SpaceRenderer(render::Camera* camera)
	: camera_{ camera }, view_{}, projection_{}, show_orbits_{ false } {
	shader_ = new render::shader::ShaderProgram{"shaders/vertex.glsl", "shaders/fragment.glsl"};
	shader_->link();
	star_shader_ = new render::shader::ShaderProgram{"shaders/star_vertex.glsl", "shaders/star_geometry.glsl", "shaders/star_fragment.glsl"};
	star_shader_->link();
}

/*
 * SpaceRenderer preRender:
 * - Adapt the camera to the window size.
 * - Get the view and projection matrix.
 * - Set the viewport.
 * - Clear the screen if it is set to.
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
}

/*
 * SpaceRenderer preRender:
 * - Disable OpenGL pipline operations.
 * - Swap frame for smooth transition.
 * - Poll all queued events.
 */
void SpaceRenderer::postRender() {
	camera_->unlock();

	glfwSwapBuffers(glfwGetCurrentContext());
	glfwPollEvents();
}

/*
 * SpaceRenderer render SpaceSimulation:
 * - Do prerender operation.
 * - Render solar system.s
 * - Render stars.
 * - Do postrender operation.
 */
void SpaceRenderer::render(simulation::SpaceSimulation* space_simulation) {
	preRender();

	glEnable(GL_DEPTH_TEST);

	star_shader_->use();

	for (simulation::object::Stars* stars : space_simulation->getGroupOfStars())
		render(stars);
	
	shader_->use();

	for (simulation::object::OrbitalSystem* solar_system : space_simulation->getSolarSystems())
		render(solar_system);

	glDisable(GL_DEPTH_TEST);

	postRender();
}

/*
 * SpaceRenderer render Stars:
 * - For each physical star model.
 *   - Bind the model
 *   - Calculate transformation matrix for the model.
 *   - Set uniforms.
 *   - Enable OpenGL pipline operations.
 *   - Render.
 *   - Disable OpenGL pipline operations.
 *   - unbind model.
 */
void SpaceRenderer::render(simulation::object::Stars* stars) {
	simulation::object::AggregateObject* star_object{ stars->getPhysicalObject() };

	glm::vec3 camera_position{ camera_->getPosition() };
	glm::vec3 relative_position{ camera_position / stars->getDistance() };
	glm::mat4 view{ glm::lookAt(relative_position, relative_position + camera_->getDirection(), camera_->getUp()) };
	glm::mat4 proj{ glm::perspective(glm::radians(camera_->getFieldOfView()), camera_->getAspectRatio(), 10.0f, 100000.0f) };
	glm::mat4 agg_model{ glm::translate(glm::mat4{1.0f}, relative_position) /* * star_object->getMatrix()*/ };
	for (std::pair<simulation::object::Object*, glm::vec3> object : star_object->getObjects()) {
		object.first->bind();

		glm::mat4 obj_model{ glm::mat4{ 1.0f } /* * glm::translate(glm::mat4{1.0f}, object.second) * object.first->getMatrix()*/ };
		star_shader_->setUniform(view, "view");
		star_shader_->setUniform(proj, "projection");
		star_shader_->setUniform(static_cast<float>(glfwGetTime()), "time_point");
		star_shader_->setUniform(camera_->getAspectRatio(), "aspect_ratio");

		glDepthRange(1.0f, 1.0f);
		glDepthFunc(GL_LEQUAL);
		//glPointSize(stars->getSize()  * (render_width_ / 2000.0f) * sqrt(60.0f / camera_->getFieldOfView()) );
		
		glDrawArrays(GL_POINTS, 0, object.first->getModel()->size());
		
		glDepthFunc(GL_LESS);
		glDepthRange(0.0f, 1.0f);

		object.first->unbind();
	}
}

glm::vec3 getOrbitColor(simulation::object::AstronomicalObject* object) {
	glm::vec3 color{1.0f};
	const std::type_info& object_type{ typeid(*object) };
	if (object_type == typeid(simulation::object::Solar))
		color = dynamic_cast<simulation::object::Solar*>(object)->getColor();
	else if (object_type == typeid(simulation::object::Planet))
		color = dynamic_cast<simulation::object::Planet*>(object)->getColor();
	else if (object_type == typeid(simulation::object::OrbitalSystem))
		color = getOrbitColor(dynamic_cast<simulation::object::OrbitalSystem*>(object)->getOrbits().at(0).first);

	return color;
}

/*
 * SpaceRenderer render OrbitalSystem:
 * - Calculate system matrix.
 * - Set uniforms.
 * - Foreach orbit:
 *   - Render the object in orbit.
 *   - Render the orbit.
 */
void SpaceRenderer::render(simulation::object::OrbitalSystem* orbital_system, glm::mat4 transform) {
	glm::mat4 system_matrix{ transform * orbital_system->getMatrix() };

	shader_->setUniform(glm::vec3(0.0f, 0.0f, 0.0f), "light_position");
	shader_->setUniform(camera_->getPosition(), "camera_position");

	for (std::pair<simulation::object::AstronomicalObject*, simulation::object::Orbit*> orbit : orbital_system->getOrbits()) {
		render(orbit.first, system_matrix * orbit.second->getMatrix());		
		if (showOrbits()) {
			render(orbit.second, getOrbitColor(orbit.first), system_matrix);
		}
	}
}

/*
 * SpaceRenderer render Orbit:
 * - Calculate transformation matrix for the models.
 * - For each physical solar model.
 *   - Bind the model
 *   - Set uniforms.
 *   - Render.
 *   - unbind model.
 */
void SpaceRenderer::render(simulation::object::Orbit* orbit, glm::vec3 color, glm::mat4 transform) {
	simulation::object::AggregateObject* orbit_object{ orbit->getPhysicalObject() };

	glm::mat4 agg_model{ transform * orbit->getOrbitMatrix() };
	for (std::pair<simulation::object::Object*, glm::vec3> object : orbit_object->getObjects()) {
		object.first->bind();

		glm::mat4 obj_model{ agg_model * glm::translate(glm::mat4{1.0f}, object.second) * object.first->getMatrix() };
		shader_->setUniform(obj_model, "model");
		shader_->setUniform(projection_ * view_ * obj_model, "MVP");
		shader_->setUniform(color, "color");
		shader_->setUniform(true, "is_sun");

		glDrawArrays(GL_LINES, 0, object.first->getModel()->size());
		object.first->unbind();
	}
}

/*
 * SpaceRenderer render Solar:
 * - Calculate transformation matrix for the models.
 * - For each physical solar model.
 *   - Bind the model
 *   - Set uniforms.
 *   - Render.
 *   - unbind model.
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
 * - Calculate transformation matrix for the models.
 * - For each physical planet model.
 *   - Bind the model
 *   - Set uniforms.
 *   - Render.
 *   - unbind model.
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

bool SpaceRenderer::showOrbits() {
	return getShowOrbits();
}

void SpaceRenderer::setShowOrbits(bool show_orbits) {
	show_orbits_ = show_orbits;
}

bool SpaceRenderer::getShowOrbits() {
	return show_orbits_;
}

} // namespace render
} // namespace wanderers
