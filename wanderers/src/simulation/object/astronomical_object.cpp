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

AstronomicalObject::AstronomicalObject(AbstractObject abstract_object, AggregateObject* physical_object) 
	: AstronomicalObject{abstract_object, physical_object, 0.0f, 0.0f, kUp, kFace} {}

AstronomicalObject::AstronomicalObject(AbstractObject abstract_object, AggregateObject* physical_object, 
									   float rotational_angle, float angular_velocity, 
									   glm::vec3 rotational_axis, glm::vec3 rotational_face)
	: AbstractObject{ abstract_object }, physical_object_{ physical_object }, 
	  rotational_angle_{ rotational_angle }, angular_velocity_{ angular_velocity },
	  rotational_axis_{ glm::normalize(rotational_axis) }, 
	  rotational_face_{ orthogonalize(rotational_axis_, glm::normalize(rotational_face)) },
	  rotational_side_{ glm::cross(rotational_axis_, rotational_face_) } {}

void AstronomicalObject::setPhysicalObject(AggregateObject* physical_object) {
	physical_object_ = physical_object;
}

AggregateObject* const AstronomicalObject::getPhysicalObject() {
	return physical_object_;
}

void AstronomicalObject::setRotationalAxis(glm::vec3 rotational_axis) {
	if (glm::normalize(rotational_axis) != rotational_axis_) {
		glm::mat4 rotation{ glm::orientation(rotational_axis_, glm::normalize(rotational_axis)) };
		rotational_axis_ = glm::normalize(rotational_axis);
		rotational_face_ = rotation * glm::vec4{ rotational_face_, 0.0f };
		rotational_side_ = rotation * glm::vec4{ rotational_side_, 0.0f };
	}
}

glm::vec3 AstronomicalObject::getRotationalAxis() {
	return rotational_axis_;
}

void AstronomicalObject::setRotationalFace(glm::vec3 rotational_face) {
	if (glm::normalize(rotational_face) != rotational_face_) {
		glm::mat4 rotation{ glm::orientation(rotational_face_, glm::normalize(rotational_face)) };
		rotational_face_ = glm::normalize(rotational_face);
		rotational_side_ = rotation * glm::vec4{ rotational_side_, 0.0f };
		rotational_axis_ = rotation * glm::vec4{ rotational_axis_, 0.0f };
	}
}

glm::vec3 AstronomicalObject::getRotationalFace() {
	return rotational_face_;
}

void AstronomicalObject::setRotationalSide(glm::vec3 rotational_side) {
	if (glm::normalize(rotational_side) != rotational_side_) {
		glm::mat4 rotation{ glm::orientation(rotational_side_, glm::normalize(rotational_side)) };
		rotational_side_ = glm::normalize(rotational_side);
		rotational_axis_ = rotation * glm::vec4{ rotational_axis_, 0.0f };
		rotational_face_ = rotation * glm::vec4{ rotational_face_, 0.0f };
	}
}

glm::vec3 AstronomicalObject::getRotationalSide() {
	return rotational_side_;
}

void AstronomicalObject::setRotationalAngle(float rotational_angle) {
	rotational_angle_ = rotational_angle;
}

float AstronomicalObject::getRotationalAngle() {
	return rotational_angle_;
}

void AstronomicalObject::setAngularVelocity(float angular_velocity) {
	angular_velocity_ = angular_velocity;
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
	if (physical_object_ != nullptr) {
		physical_object_->elapseTime(seconds);
	}
}

} // namespace object
} // namespace simulation
} // namespace wanderers
