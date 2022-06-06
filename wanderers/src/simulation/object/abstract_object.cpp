/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the abstract class for objects.                         *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/abstract_object.h"

#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

namespace wanderers {
namespace simulation {
namespace object {

std::uint64_t AbstractObject::id_counter_{0};

glm::vec3 orthogonalize(glm::vec3 v1, glm::vec3 v2) {
	glm::vec3 normal{ glm::cross(v1, v2) };
	return glm::rotate(glm::normalize(v1), glm::radians(90.0f), glm::normalize(normal));
}

AbstractObject::AbstractObject(glm::vec3 position, glm::vec3 orientation, glm::vec3 face, float radius) 
	: position_{ position }, orientation_{ glm::normalize(orientation) }, face_{ orthogonalize(orientation_, glm::normalize(face)) },
	side_{ glm::cross(orientation_, face_) }, radius_{ radius }, parent_{nullptr}, object_id_{ id_counter_++ } {}

std::uint64_t AbstractObject::getObjectId() const {
	return object_id_;
}

void AbstractObject::setPosition(glm::vec3 position) {
	position_ = position;
}

glm::vec3 AbstractObject::getPosition() const {
	return position_;
}

void AbstractObject::setOrientation(glm::vec3 orientation) {
	if (glm::normalize(orientation) != orientation_) {
		glm::mat4 rotation{ glm::orientation(orientation_, glm::normalize(orientation)) };
		orientation_ = glm::normalize(orientation);
		face_ = rotation * glm::vec4{ face_, 0.0f };
		side_ = rotation * glm::vec4{ side_, 0.0f };
	}
}

glm::vec3 AbstractObject::getOrientation() const {
	return orientation_;
}

void AbstractObject::setFace(glm::vec3 face) {
	if (glm::normalize(face) != face_) {
		glm::mat4 rotation{ glm::orientation(face_, glm::normalize(face)) };
		face_ = glm::normalize(face);
		side_ = rotation * glm::vec4{ side_, 0.0f };
		orientation_ = rotation * glm::vec4{ orientation_, 0.0f };
	}
}

glm::vec3 AbstractObject::getFace() const {
	return face_;
}

void AbstractObject::setSide(glm::vec3 side) {
	if (glm::normalize(side) != side_) {
		glm::mat4 rotation{ glm::orientation(side_, glm::normalize(side)) };
		side = glm::normalize(side);
		orientation_ = rotation * glm::vec4{ orientation_, 0.0f };
		face_ = rotation * glm::vec4{ face_, 0.0f };
	}
}

glm::vec3 AbstractObject::getSide() const {
	return side_;
}

void AbstractObject::setRadius(float radius) {
	radius_ = radius;
}

float AbstractObject::getRadius() const {
	return radius_;
}

void AbstractObject::setParent(AbstractObject* parent) {
	parent_ = parent;
}

AbstractObject* AbstractObject::getParent() const {
	return parent_;
}

glm::mat4 AbstractObject::getMatrix() {
	glm::mat4 scale_matrix{ glm::scale(glm::mat4{1.0f}, glm::vec3{radius_})};
	
	glm::mat4 orientation_matrix{ kUp == -orientation_ ? glm::rotate(glm::mat4{1.0f}, glm::radians(180.0f), face_) : glm::orientation(kUp, orientation_) };
	glm::mat4 face_matrix{ kFace == -face_ ? glm::rotate(glm::mat4{1.0f}, glm::radians(180.0f), orientation_) : glm::orientation(kFace, face_) };

	return scale_matrix * face_matrix * orientation_matrix;
}

void AbstractObject::elapseTime(double seconds) {}

} // namespace object
} // namespace simulation
} // namespace wanderers
