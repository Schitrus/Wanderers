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

namespace wanderers {
namespace simulation {
namespace object {

Orbit::Orbit(AstronomicalObject* object) : orbitor_{ object }, 
                                          radius_{ kDefaultRadius },
                                          angular_velocity_{kDefaultAngularVelocity},
                                          orbital_axis_{ kDefaultOrbitalAxis },
                                          orbital_angle_{kDefaultStartingOrbitalAngle} {}

Orbit::Orbit(AstronomicalObject* object, float radius, float angular_velocity, glm::vec3 orbital_axis, float orbital_angle) 
            : orbitor_{ object },
              radius_{ radius },
              angular_velocity_{ angular_velocity },
              orbital_axis_{ orbital_axis },
              orbital_angle_{ orbital_angle } {}

/*
 * Orbit Destructor:
 * - Destroy the orbitee.
 */
Orbit::~Orbit() {
    delete orbitor_; // might be not good practice.
}

AstronomicalObject* Orbit::getOrbitor() {
    return orbitor_;
}

/*
 * Orbit getOrbitMatrix:
 * - Create matrix (matrix multiplication reverse order):
 *   - Move position to orbit distance.
 *   - Rotate position along current orbit angle.
 *   - Rotate whole orbit position along the orbital axis.
 */
glm::mat4 Orbit::getOrbitMatrix() {
    glm::mat4 orbit_matrix = glm::rotate(glm::mat4{ 1.0f }, glm::radians(orbital_angle_), orbital_axis_)
                           * glm::translate(glm::mat4{ 1.0f }, radius_ * glm::vec3{ 1.0f, 0.0f, 0.0f });
    return orbit_matrix;
}

/*
 * Orbit elapseTime:
 * - Increase orbital angle as much as the angular velocity.
 * - Elapse the time for the Orbitor.
 */
void Orbit::elapseTime(double seconds) {
    orbital_angle_ += angular_velocity_ * seconds;
    orbital_angle_ = fmod(orbital_angle_, 360.0f);

    orbitor_->elapseTime(seconds);
}

} // namespace simulation
} // namespace object
} // namespace wanderers
