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

AbstractObject::AbstractObject() : AbstractObject(kOrigo, kUp, kFace, kIdentityScale) {}

AbstractObject::AbstractObject(glm::vec3 position, glm::vec3 orientation, glm::vec3 face, glm::vec3 scale, AbstractObject* parent) 
	: position_{ position }, orientation_{ orientation, face }, scale_{ scale }, object_id_{ id_counter_++ } {}

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
	orientation_.setNormal(orientation);
}

glm::vec3 AbstractObject::getOrientation() const {
	return orientation_.getNormal();
}

void AbstractObject::setFace(glm::vec3 face) {
	orientation_.setTangent(face);
}

glm::vec3 AbstractObject::getFace() const {
	return orientation_.getTangent();
}

void AbstractObject::setSide(glm::vec3 side) {
	orientation_.setBitangent(side);
}

glm::vec3 AbstractObject::getSide() const {
	return orientation_.getBitangent();
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
	
	//glm::mat4 orientation_matrix{ kUp == -getOrientation() ? glm::rotate(glm::mat4{1.0f}, glm::radians(180.0f), getFace()) : glm::orientation(kUp, getOrientation()) };
	//glm::mat4 face_matrix{ kFace == -getFace() ? glm::rotate(glm::mat4{1.0f}, glm::radians(180.0f), getOrientation()) : glm::orientation(kFace, getFace()) };

	return translation_matrix * scale_matrix * orientation_.orientationMatrix(common::kYOrientation);
}

void AbstractObject::elapseTime(double seconds) {}

} // namespace object
} // namespace simulation
} // namespace wanderers
