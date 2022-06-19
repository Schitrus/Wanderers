/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the class for representation of an astronomical         *
 *   object in orbit.                                                        *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/orbit.h"

/* External Includes */
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

#include <iostream>

namespace wanderers {
namespace simulation {
namespace object {

Orbit::Orbit(float eccentricity, float semimajor_axis, float inclination, float longitude_of_acending_node, float argument_of_periapsis, float true_anomaly, float angular_velocity)
    : Orbit{semimajor_axis, 
            semimajor_axis * sqrt(1 - eccentricity * eccentricity),
            angular_velocity,
            true_anomaly, 
            glm::rotate(object::AbstractObject::kUp, 
                        inclination, 
                        // Acending node
                        glm::rotate(object::AbstractObject::kFace, 
                                    longitude_of_acending_node, 
                                    object::AbstractObject::kUp)),
            glm::rotate(// Acending node
                        glm::rotate(object::AbstractObject::kFace,
                                    longitude_of_acending_node,
                                    object::AbstractObject::kUp),
                        argument_of_periapsis,
                        // axis
                        glm::rotate(object::AbstractObject::kUp, 
                                    inclination, 
                                    // Acending node
                                    glm::rotate(object::AbstractObject::kFace, 
                                                longitude_of_acending_node, 
                                                object::AbstractObject::kUp))) } {}

Orbit::Orbit(float radius, float angular_velocity, float orbital_angle) 
    : Orbit(radius, radius, angular_velocity, orbital_angle) {}

Orbit::Orbit(float major_axis, float minor_axis, float angular_velocity, float orbital_angle) 
    : Orbit(major_axis, minor_axis, angular_velocity, orbital_angle, kUp, kFace) {}

Orbit::Orbit(float radius, float angular_velocity, float orbital_angle, glm::vec3 orbital_axis, glm::vec3 orbital_face) 
    : Orbit{radius, radius, angular_velocity, orbital_angle, orbital_axis, orbital_face} {}
Orbit::Orbit(float major_axis, float minor_axis, float angular_velocity, float orbital_angle, glm::vec3 orbital_axis, glm::vec3 orbital_face) 
    : Orbit{ kAbstractAstronomicalObject, major_axis, minor_axis, angular_velocity, orbital_angle, orbital_axis, orbital_face } {}

Orbit::Orbit(AstronomicalObject astronomical_object, float major_axis, float minor_axis, 
             float angular_velocity, float orbital_angle, glm::vec3 orbital_axis, glm::vec3 orbital_face)
            : AstronomicalObject{ astronomical_object },
              major_axis_{ major_axis }, minor_axis_{ minor_axis },
              angular_velocity_{ angular_velocity }, orbital_angle_{ orbital_angle },
              orbital_axis_{ glm::normalize(orbital_axis) },
              orbital_face_{ orthogonalize(orbital_face, orbital_axis_) },
              orbital_side_{ glm::cross(orbital_axis_, orbital_face_) }{}

void Orbit::setMajorAxis(float major_axis) {
    major_axis_ = major_axis;
}
float Orbit::getMajorAxis() {
    return major_axis_;
}

void Orbit::setMinorAxis(float minor_axis) {
    minor_axis_ = minor_axis;
}
float Orbit::getMinorAxis() {
    return minor_axis_;
}

void Orbit::setAngularVelocity(float angular_velocity) {
    angular_velocity_ = angular_velocity;
}

float Orbit::getAngularVelocity() {
    return angular_velocity_;
}

void Orbit::setOrbitalAngle(float orbital_angle) {
    orbital_angle_ = orbital_angle;
}

float Orbit::getOrbitalAngle() {
    return orbital_angle_;
}

void Orbit::setOrbitalAxis(glm::vec3 orbital_axis) {
    if (glm::normalize(orbital_axis) != orbital_axis_) {
        glm::mat4 rotation{ glm::orientation(orbital_axis_, glm::normalize(orbital_axis)) };
        orbital_axis_ = glm::normalize(orbital_axis);
        orbital_face_ = rotation * glm::vec4{ orbital_face_, 0.0f };
        orbital_side_ = rotation * glm::vec4{ orbital_side_, 0.0f };
    }
}

glm::vec3 Orbit::getOrbitalAxis() {
    return orbital_axis_;
}

void Orbit::setOrbitalFace(glm::vec3 orbital_face) {
    if (glm::normalize(orbital_face) != orbital_face_) {
        glm::mat4 rotation{ glm::orientation(orbital_face_, glm::normalize(orbital_face)) };
        orbital_face_ = glm::normalize(orbital_face);
        orbital_side_ = rotation * glm::vec4{ orbital_side_, 0.0f };
        orbital_axis_ = rotation * glm::vec4{ orbital_axis_, 0.0f };
    }
}

glm::vec3 Orbit::getOrbitalFace() {
    return orbital_face_;
}

void Orbit::setOrbitalSide(glm::vec3 orbital_side) {
    if (glm::normalize(orbital_side) != orbital_side_) {
        glm::mat4 rotation{ glm::orientation(orbital_side_, glm::normalize(orbital_side)) };
        orbital_side_ = glm::normalize(orbital_side);
        orbital_axis_ = rotation * glm::vec4{ orbital_axis_, 0.0f };
        orbital_face_ = rotation * glm::vec4{ orbital_face_, 0.0f };
    }
}

glm::vec3 Orbit::getOrbitalSide() {
    return orbital_side_;
}

/*
 * Orbit getOrbitMatrix:
 * - Create matrix (matrix multiplication reverse order):
 *   - Move position to orbit distance.
 *   - Rotate position along current orbit angle.
 *   - Rotate whole orbit position along the orbital axis.
 */
glm::mat4 Orbit::getOrbitMatrix() {
    glm::mat4 orientation_matrix{ kUp == -orbital_axis_ ? glm::rotate(glm::mat4{1.0f}, glm::radians(180.0f), kFace) : glm::rotation(kUp, orbital_axis_) };
    glm::vec3 oriented_face{ orientation_matrix * glm::vec4{ kFace, 0.0f } };
    glm::mat4 face_matrix{ oriented_face == -orbital_face_ ? glm::rotate(glm::mat4{1.0f}, glm::radians(180.0f), orbital_axis_) : glm::rotation(oriented_face, orbital_face_) };
    float normalized_major = minor_axis_ > 0.00001f ? major_axis_ / minor_axis_ : 1.0f;
    float focus_point = sqrt(normalized_major * normalized_major - 1.0f);
    return face_matrix * orientation_matrix
                       * glm::translate(glm::mat4{ 1.0f }, -kFace * minor_axis_ * focus_point)
                       * glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 1.0f, 1.0f, normalized_major })
                       * glm::rotate(glm::mat4{ 1.0f }, glm::radians(orbital_angle_), kUp) 
                       * glm::translate(glm::mat4{ 1.0f }, kFace * minor_axis_) 
                       * glm::rotate(glm::mat4{ 1.0f }, -glm::radians(orbital_angle_), kUp) 
                       * glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f / normalized_major });
}

glm::mat4 Orbit::getMatrix() {
    return getOrbitMatrix();
}

/*
 * Orbit elapseTime:
 * - Increase orbital angle as much as the angular velocity.
 * - Elapse the time for the Orbitor.
 */
void Orbit::elapseTime(double seconds) {
    float normalized_major = minor_axis_ > 0.00001f ? major_axis_ / minor_axis_ : 1.0f;
    float focus_point = sqrt(normalized_major * normalized_major - 1.0f);

    float dist = sqrt(pow(normalized_major * cos(glm::radians(orbital_angle_)) + focus_point, 2.0f) + pow(sin(glm::radians(orbital_angle_)), 2.0f));

    orbital_angle_ += angular_velocity_ / sqrt(dist) * seconds;
    orbital_angle_ = fmod(orbital_angle_, 360.0f);
} 

} // namespace object
} // namespace simulation
} // namespace wanderers
