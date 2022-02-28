#include "Solar.h"

#include "glm/ext.hpp"

#include <iostream>

Solar::Solar() : temperature_{ kDefaultTemperature }, 
                 radius_{ kDefaultRadius },
                 angular_velocity_{ kDefaultAngularVelocity },
                 rotational_axis_{ kDefaultRotationalAxis },
                 rotational_angle_{ kDefaultStartingRotationalAngle } {}

Solar::Solar(float temperature, float radius,
             float angular_velocity, glm::vec3 rotational_axis, float rotational_angle)
             : temperature_{ temperature },
               radius_{ radius },
               angular_velocity_{ angular_velocity },
               rotational_axis_{ rotational_axis },
               rotational_angle_{ rotational_angle } {}

void Solar::elapseTime(double seconds) {
    rotational_angle_ += angular_velocity_ * seconds;
    rotational_angle_ = fmod(rotational_angle_, 360.0f);
}

glm::vec3 Solar::getColor() {
    glm::vec3 color;
    if (temperature_ <= 6600) {
        color.r = 1.0;
        color.g = glm::clamp((99.4708025861 * log(temperature_/100.0) - 161.1195681661) / 255.0, 0.0, 1.0);
        if (temperature_ <= 1900) {
            color.b = 0.0;
        }
        else {
            color.b = glm::clamp((138.5177312231 * log(temperature_/100 - 10.0) - 305.0447927307) / 255.0, 0.0, 1.0);
        }
    }
    else {
        color.r = glm::clamp((329.698727446 * pow(temperature_/100.0 - 60.0, -0.1332047592)) / 255.0, 0.0, 1.0);
        color.g = glm::clamp((288.1221695283 * pow(temperature_ /100.0 - 60.0, -0.0755148492)) / 255.0, 0.0, 1.0);
        color.b = 1.0;
    }
    return color;
}

glm::mat4 Solar::getSolarMatrix() {
    glm::mat4 solar_matrix = glm::rotate(glm::mat4{ 1.0f }, glm::radians(rotational_angle_), rotational_axis_)
                           * glm::scale(glm::mat4{ 1.0f }, glm::vec3{ radius_ });
    return solar_matrix;
}