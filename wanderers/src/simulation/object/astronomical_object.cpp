/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of base class for representation of astronomical objects.  *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/astronomical_object.h"

/* External Includes */
#include "glm/gtx/rotate_vector.hpp"

/* Internal Includes */
#include "simulation/object/model/icosahedron.h"

/* STL Includes */
#include <algorithm>

namespace wanderers {
namespace simulation {
namespace object {

AstronomicalObject::AstronomicalObject(AbstractObject abstract_object, Object* physical_object, glm::vec3 rotational_axis, float rotational_angle, float angular_velocity)
	: AbstractObject{ abstract_object }, 
	  physical_object_{ physical_object }, rotational_axis_{ rotational_axis }, 
	  rotational_angle_{ rotational_angle }, angular_velocity_{ angular_velocity } {}

Object* const AstronomicalObject::getPhysicalObject() {
	return physical_object_;
}

glm::vec3 AstronomicalObject::getRotationalAxis() {
	return rotational_axis_;
}

float AstronomicalObject::getRotationalAngle() {
	return rotational_angle_;
}

float AstronomicalObject::getAngularVelocity() {
	return angular_velocity_;
}

glm::mat4 AstronomicalObject::getRotationalMatrix() {
	return glm::rotate(glm::mat4{ 1.0f }, glm::radians(rotational_angle_), rotational_axis_);
}

glm::mat4 AstronomicalObject::getMatrix() {
	return getRotationalMatrix() * AbstractObject::getMatrix();
}

void AstronomicalObject::elapseTime(double seconds) {
	rotational_angle_ += angular_velocity_ * seconds;
	rotational_angle_ = fmod(rotational_angle_, 360.0f);
	physical_object_->elapseTime(seconds);
}

} // namespace object
} // namespace simulation
} // namespace wanderers
