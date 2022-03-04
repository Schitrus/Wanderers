#include "simulation/object/orbit.h"

/* External Includes */
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"

/* STL Includes */
#include <iostream>

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

Orbit::~Orbit() {
    delete orbitor_;
}

AstronomicalObject* Orbit::getOrbitor() {
    return orbitor_;
}

glm::mat4 Orbit::getOrbitMatrix() {
    glm::mat4 orbit_matrix = glm::orientation(orbital_axis_, glm::vec3{0.0f, 1.0f, 0.0f})
                           * glm::rotate(glm::mat4{ 1.0f }, glm::radians(orbital_angle_), orbital_axis_) // Fix maybe orientation?
                           * glm::translate(glm::mat4{ 1.0f }, radius_ * glm::vec3{ 1.0f, 0.0f, 0.0f });
    return orbit_matrix;
}

void Orbit::elapseTime(double seconds) {
    orbital_angle_ += angular_velocity_ * seconds;
    orbital_angle_ = fmod(orbital_angle_, 360.0f);

    orbitor_->elapseTime(seconds);
}