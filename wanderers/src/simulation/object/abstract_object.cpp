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

glm::vec3 AbstractObject::orthogonalize(glm::vec3 vector, glm::vec3 normal) {
	glm::vec3 cross_normal{ glm::cross(glm::normalize(vector), glm::normalize(normal)) };
	return glm::cross(glm::normalize(normal), glm::normalize(cross_normal));
}

AbstractObject::AbstractObject() : AbstractObject(kOrigo, kUp, kFace, kIdentityScale) {}

AbstractObject::AbstractObject(glm::vec3 position, glm::vec3 orientation, glm::vec3 face, glm::vec3 scale, AbstractObject* parent) 
	: position_{ position }, orientation_{ glm::normalize(orientation) }, face_{ orthogonalize(face, orientation_) },
	side_{ glm::cross(orientation_, face_) }, scale_{ scale }, object_id_{ id_counter_++ } {}

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

void AbstractObject::setScale(glm::vec3 scale) {
	scale_ = scale;
}

glm::vec3 AbstractObject::getScale() const {
	return scale_;
}

glm::mat4 AbstractObject::getMatrix() {
	glm::mat4 translation_matrix{ glm::translate(glm::mat4{1.0f}, position_) };
	glm::mat4 scale_matrix{ glm::scale(glm::mat4{1.0f}, scale_)};
	
	glm::mat4 orientation_matrix{ kUp == -orientation_ ? glm::rotate(glm::mat4{1.0f}, glm::radians(180.0f), face_) : glm::orientation(kUp, orientation_) };
	glm::mat4 face_matrix{ kFace == -face_ ? glm::rotate(glm::mat4{1.0f}, glm::radians(180.0f), orientation_) : glm::orientation(kFace, face_) };

	return translation_matrix * scale_matrix * face_matrix * orientation_matrix;
}

void AbstractObject::elapseTime(double seconds) {}

} // namespace object
} // namespace simulation
} // namespace wanderers
