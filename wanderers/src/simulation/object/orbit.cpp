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
    : Orbit{ getOrbitTrailObject(), major_axis, minor_axis, angular_velocity, orbital_angle, orbital_axis, orbital_face } {}

Orbit::Orbit(AstronomicalObject astronomical_object, float major_axis, float minor_axis, 
             float angular_velocity, float orbital_angle, glm::vec3 orbital_axis, glm::vec3 orbital_face)
            : AstronomicalObject{ astronomical_object },
              major_axis_{ major_axis }, minor_axis_{ minor_axis },
              angular_velocity_{ angular_velocity }, orbital_angle_{ orbital_angle },
              orbital_orientation_{orbital_axis, orbital_face} {}

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
    orbital_orientation_.setNormal(orbital_axis);
}

glm::vec3 Orbit::getOrbitalAxis() const {
    return orbital_orientation_.getNormal();
}

void Orbit::setOrbitalFace(glm::vec3 orbital_face) {
    orbital_orientation_.setTangent(orbital_face);

}

glm::vec3 Orbit::getOrbitalFace() const {
    return orbital_orientation_.getNormal();
}

void Orbit::setOrbitalSide(glm::vec3 orbital_side) {
    orbital_orientation_.setBitangent(orbital_side);

}

glm::vec3 Orbit::getOrbitalSide() const {
    return orbital_orientation_.getBitangent();
}

/*
 * Orbit getOrbitMatrix:
 * - Invert scaling to avoid ellipse artifacts.
 * - Invert rotate to avoid rotating the object in orbit.
 * - Translate object to orbit distance (semi-minor axis).
 * - Rotate object around the orbit.
 * - Scale the orbit in one axis to make it elliptical.
 * - Translate orbit to focus point.
 * - Orient orbit according to oientation parameters.
 */
glm::mat4 Orbit::getOrbitMatrix() {
    float normalized_major = minor_axis_ > 0.00001f ? major_axis_ / minor_axis_ : 1.0f;
    float focus_point = sqrt(normalized_major * normalized_major - 1.0f);
    return orbital_orientation_.orientationMatrix(common::kYOrientation)
           * glm::translate(glm::mat4{ 1.0f }, -kFace * minor_axis_ * focus_point)
           * glm::scale(glm::mat4{ 1.0f }, glm::vec3{ minor_axis_, 1.0f, major_axis_ })
           * glm::rotate(glm::mat4{ 1.0f }, glm::radians(0.5f * orbital_angle_), kUp);
}

/*
 * Orbit getMatrix:
 * - Invert scaling to avoid ellipse artifacts.
 * - Invert rotate to avoid rotating the object in orbit.
 * - Translate object to orbit distance (semi-minor axis).
 * - Rotate object around the orbit.
 * - Scale the orbit in one axis to make it elliptical.
 * - Translate orbit to focus point.
 * - Orient orbit according to oientation parameters.
 */
glm::mat4 Orbit::getMatrix() {
    float normalized_major = minor_axis_ > 0.00001f ? major_axis_ / minor_axis_ : 1.0f;
    float focus_point = sqrt(normalized_major * normalized_major - 1.0f);
    return orbital_orientation_.orientationMatrix(common::kYOrientation)
        * glm::translate(glm::mat4{ 1.0f }, -kFace * minor_axis_ * focus_point)
        * glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 1.0f, 1.0f, normalized_major })
        * glm::rotate(glm::mat4{ 1.0f }, glm::radians(orbital_angle_), kUp)
        * glm::translate(glm::mat4{ 1.0f }, kFace * minor_axis_)
        * glm::rotate(glm::mat4{ 1.0f }, -glm::radians(orbital_angle_), kUp)
        * glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f / normalized_major });
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
