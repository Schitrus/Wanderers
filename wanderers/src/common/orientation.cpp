/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Orientation class.                                  *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "common/orientation.h"

#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

namespace wanderers {
namespace common {


glm::vec3 Orientation::orthogonalize(glm::vec3 vector, glm::vec3 normal) {
	glm::vec3 cross_product{ glm::cross(normalize(vector), normal) };
	return glm::cross(normal, cross_product);
}

glm::vec3 Orientation::normalize(glm::vec3 vector) {
	assert((glm::length(vector) > 0.0f) && "Vector have no direction!");
	return glm::normalize(vector);
}

Orientation::Orientation() : Orientation(kUp, kFront) {}

Orientation::Orientation(glm::vec3 normal, glm::vec3 tangent) 
	: normal_{ normalize(normal) }, tangent_{ orthogonalize(tangent, normal_) }, bitangent_{ cross(normal_, tangent_) } {}

void Orientation::transform(glm::mat4 matrix, Orientation orientation) {
    auto apply_matrix = [&matrix](glm::vec3 axis) { return glm::normalize(glm::vec3{ matrix * glm::vec4{axis, 0.0f} }); };
    normal_ = apply_matrix(orientation.normal_);
    tangent_ = apply_matrix(orientation.tangent_);
    bitangent_ = apply_matrix(orientation.bitangent_);
}

glm::vec3 Orientation::translate(glm::vec3 movement) {
    return normal_ * movement.z + tangent_ * movement.y + bitangent_ * movement.x;
}

glm::vec3 Orientation::rotation(float angle_in_radians, glm::vec3 vector, glm::vec3 normal, glm::vec3& tangent, glm::vec3& bitangent) {
    tangent = glm::rotate(tangent, angle_in_radians, normal);
    bitangent = glm::rotate(bitangent, angle_in_radians, normal);
    return glm::rotate(vector, angle_in_radians, normal);
}

glm::vec3 Orientation::focusRotation(glm::vec2 angles_in_radians, glm::vec3 vector) {
    return tangentRotation(angles_in_radians.x, vector) + bitangentRotation(angles_in_radians.y, vector) - 2.0f * vector;
}

glm::vec3 Orientation::normalRotation(float angle_in_radians, glm::vec3 vector) {
    return rotation(angle_in_radians, vector, normal_, tangent_, bitangent_);
}

glm::vec3 Orientation::tangentRotation(float angle_in_radians, glm::vec3 vector) {
    return rotation(angle_in_radians, vector, tangent_, bitangent_, normal_);
}

glm::vec3 Orientation::bitangentRotation(float angle_in_radians, glm::vec3 vector) {
    return rotation(angle_in_radians, vector, bitangent_, normal_, tangent_);
}

/*
 * Orientation setAxis.
 * - If axis is zero do nothing.
 * - If axis is opposite, mirror orientation.
 * - Otherwise rotate normal and tangent with the rotation from normal to axis.
 */
void Orientation::setAxis(glm::vec3 axis, glm::vec3& normal, glm::vec3& tangent, glm::vec3& bitangent) {
    // Axis not zero
    if (glm::length(axis) > 0.0f) {
        glm::vec3 naxis = glm::normalize(axis);

        // Axis is opposite to normal
        if (glm::all(glm::equal(normal, -naxis))) {
            normal = -naxis;
            bitangent = -bitangent;
        }
        // Axis is not same as normal
        else if (glm::any(glm::notEqual(normal, naxis))) {
            glm::mat4 rotation_matrix{ glm::rotation(normal, naxis) };
            normal = naxis;
            tangent = rotation_matrix * glm::vec4{ tangent, 0.0f };
            bitangent = glm::cross(normal, tangent);
        }
    }
}

void Orientation::setNormal(glm::vec3 normal) {
    setAxis(normal, normal_, tangent_, bitangent_);
}

glm::vec3 Orientation::getNormal() const { return normal_; }

void Orientation::setTangent(glm::vec3 tangent) {
    setAxis(tangent, tangent_, bitangent_, normal_);
}

glm::vec3 Orientation::getTangent() const { return tangent_; }

void Orientation::setBitangent(glm::vec3 bitangent) {
    setAxis(bitangent, bitangent_, normal_, tangent_);
}

glm::vec3 Orientation::getBitangent() const { return bitangent_; }

glm::mat4 Orientation::orientationMatrix() {
    return orientationMatrix(kYOrientation);
}

/*
 * Orientation orientationMatrix.
 * - Calculate rotation matrix based on the normal.
 * - Rotate the tangent with the calculated matrix.
 * - Calculate rotation matrix based on the rotated tangent.
 * - Combine the rotation matrices and return it.
 */
glm::mat4 Orientation::orientationMatrix(Orientation from) {
    glm::mat4 normal_matrix{ from.normal_ == -this->normal_ ? glm::rotate(glm::mat4{1.0f}, glm::radians(180.0f), from.tangent_) : glm::rotation(from.normal_, this->normal_) };
    glm::vec3 rotated_tangent{ normal_matrix * glm::vec4{ from.tangent_, 0.0f } };
    glm::mat4 tangent_matrix{ rotated_tangent == -this->tangent_ ? glm::rotate(glm::mat4{1.0f}, glm::radians(180.0f), this->normal_) : glm::rotation(rotated_tangent, this->tangent_) };
    return tangent_matrix * normal_matrix;
}

} // namespace common
} // namespace wanderers
