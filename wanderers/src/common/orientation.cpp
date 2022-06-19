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

glm::vec3 Orientation::normalize(glm::vec3 normal) {
	assert((glm::length(normal) > 0.0f) && "Vector have no direction!");
	return glm::normalize(normal);
}

Orientation::Orientation() : Orientation(kFront, kUp) {}

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

glm::vec3 rotation(float angle_in_radians, glm::vec3 point, glm::vec3 normal, glm::vec3& tangent, glm::vec3& bitangent) {
    tangent = glm::rotate(tangent, angle_in_radians, normal);
    bitangent = glm::rotate(bitangent, angle_in_radians, normal);
    return glm::rotate(point, angle_in_radians, normal);
}

glm::vec3 Orientation::focusRotation(glm::vec2 angles_in_radians, glm::vec3 point) {
    return tangentRotation(angles_in_radians.x, point) + bitangentRotation(angles_in_radians.y, point) - 2.0f * point;
}

glm::vec3 Orientation::normalRotation(float angle_in_radians, glm::vec3 point) {
    return rotation(angle_in_radians, point, normal_, tangent_, bitangent_);
}

glm::vec3 Orientation::tangentRotation(float angle_in_radians, glm::vec3 point) {
    return rotation(angle_in_radians, point, tangent_, bitangent_, normal_);
}

glm::vec3 Orientation::bitangentRotation(float angle_in_radians, glm::vec3 point) {
    return rotation(angle_in_radians, point, bitangent_, normal_, tangent_);
}

void setAxis(glm::vec3 axis, glm::vec3& normal, glm::vec3& tangent, glm::vec3& bitangent) {
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

glm::vec3 Orientation::getNormal() { return normal_; }

void Orientation::setTangent(glm::vec3 tangent) {
    setAxis(tangent, tangent_, bitangent_, normal_);
}

glm::vec3 Orientation::getTangent() { return tangent_; }

void Orientation::setBitangent(glm::vec3 bitangent) {
    setAxis(bitangent, bitangent_, normal_, tangent_);
}

glm::vec3 Orientation::getBitangent() { return bitangent_; }

} // namespace common
} // namespace wanderers
