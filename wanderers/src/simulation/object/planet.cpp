/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the class for representation of a planet.               *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/planet.h"

/* External Includes */
#include "glm/ext.hpp"

/* Internal Includes */
#include "simulation/object/model/icosahedron.h"

namespace wanderers {
namespace simulation {
namespace object {

Planet::Planet() : AstronomicalObject{model::getIcosahedron()},
	               surface_color_{ kDefaultSurfaceColor },
				   radius_{ kDefaultRadius },
				   angular_velocity_{ kDefaultAngularVelocity },
				   rotational_axis_{ glm::normalize(kDefaultRotationalAxis) },
				   rotational_angle_{ kDefaultStartingRotationalAngle } {}

Planet::Planet(glm::vec3 surface_color, float radius,
	           float angular_velocity, glm::vec3 rotational_axis, float rotational_angle)
	           : AstronomicalObject{ model::getIcosahedron() },
	             surface_color_{ surface_color },
				 radius_{ radius },
				 angular_velocity_{ angular_velocity },
				 rotational_axis_{ glm::normalize(rotational_axis) },
				 rotational_angle_{ rotational_angle } {}

glm::vec3 Planet::getColor() { return surface_color_; }

/*
 * Planet getPlanetMatrix:
 * - Create matrix (matrix multiplication reverse order):
 *   - Scale the planet to its current size.
 *   - Rotate the planet around it's axis.
 */
glm::mat4 Planet::getPlanetMatrix() {
	glm::mat4 planet_matrix = glm::rotate(glm::mat4{ 1.0f }, glm::radians(rotational_angle_), rotational_axis_)
		                    * glm::scale(glm::mat4{ 1.0f }, glm::vec3{ radius_ });
	return planet_matrix;
}

/*
 * Planet elapseTime:
 * - Increase rotational angle as much as the angular velocity.
 */
void Planet::elapseTime(double seconds) {
	rotational_angle_ += angular_velocity_ * seconds;
	rotational_angle_ = fmod(rotational_angle_, 360.0f);
}

} // namespace object
} // namespace simulation
} // namespace wanderers
