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

Orbit::Orbit(float radius, glm::vec3 orbital_axis, float orbital_angle, float angular_velocity) : Orbit{radius, radius, orbital_axis, orbital_angle, angular_velocity} {}

Orbit::Orbit(float major_axis, float minor_axis, glm::vec3 orbital_axis, float orbital_angle, float angular_velocity)
            : AbstractObject{ glm::vec3{0.0f}, orbital_axis, kFace, minor_axis },
              orbital_axis_{ orbital_axis },
              orbital_angle_{ orbital_angle },
              angular_velocity_{ angular_velocity },
              major_axis_{ major_axis },
              minor_axis_{ minor_axis } {}



glm::vec3 Orbit::getOrbitalAxis() {
    return orbital_axis_;
}

float Orbit::getAngle() {
    return orbital_angle_;
}

/*
 * Orbit getOrbitMatrix:
 * - Create matrix (matrix multiplication reverse order):
 *   - Move position to orbit distance.
 *   - Rotate position along current orbit angle.
 *   - Rotate whole orbit position along the orbital axis.
 */
glm::mat4 Orbit::getOrbitMatrix() {
    glm::mat4 orientation_matrix{ kUp == -orbital_axis_ ? glm::rotate(glm::mat4{1.0f}, glm::radians(180.0f), kFace) : glm::orientation(kUp, orbital_axis_) };
    float normalized_major = minor_axis_ > 0.00001f ? major_axis_ / minor_axis_ : 1.0f;
    float focus_point = sqrt(normalized_major * normalized_major - 1.0f);
    return orientation_matrix * glm::translate(glm::mat4{ 1.0f }, kSide * radius_ * focus_point)
                              * glm::scale(glm::mat4{ 1.0f }, glm::vec3{ normalized_major, 1.0f, 1.0f })
                              * glm::rotate(glm::mat4{ 1.0f }, glm::radians(orbital_angle_), kUp) 
                              * glm::translate(glm::mat4{ 1.0f }, kSide * radius_) 
                              * glm::rotate(glm::mat4{ 1.0f }, -glm::radians(orbital_angle_), kUp) 
                              * glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 1.0f / normalized_major, 1.0f, 1.0f });
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
