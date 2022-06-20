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
	  rotational_orientation_{rotational_axis, rotational_face},
	  parent_{nullptr} {}

void AstronomicalObject::setPhysicalObject(AggregateObject* physical_object) {
	physical_object_ = physical_object;
}

AggregateObject* const AstronomicalObject::getPhysicalObject() {
	return physical_object_;
}

void AstronomicalObject::setRotationalAxis(glm::vec3 rotational_axis) {
	rotational_orientation_.setNormal(rotational_axis);
}

glm::vec3 AstronomicalObject::getRotationalAxis() const {
	return rotational_orientation_.getNormal();
}

void AstronomicalObject::setRotationalFace(glm::vec3 rotational_face) {
	rotational_orientation_.setTangent(rotational_face);

}

glm::vec3 AstronomicalObject::getRotationalFace() const {
	return rotational_orientation_.getTangent();
}

void AstronomicalObject::setRotationalSide(glm::vec3 rotational_side) {
	rotational_orientation_.setBitangent(rotational_side);
}

glm::vec3 AstronomicalObject::getRotationalSide() const {
	return rotational_orientation_.getBitangent();
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

void AstronomicalObject::setParent(AstronomicalObject* parent) {
	parent_ = parent;
}

AstronomicalObject* AstronomicalObject::getParent() const {
	return parent_;
}

glm::mat4 AstronomicalObject::getRotationalMatrix() {
	return glm::rotate(glm::mat4{ 1.0f }, glm::radians(rotational_angle_), getRotationalAxis());
}

glm::mat4 AstronomicalObject::getPhysicalMatrix() {
	return AbstractObject::getMatrix();
}

glm::mat4 AstronomicalObject::getMatrix() {
	return getRotationalMatrix() * getPhysicalMatrix();
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
