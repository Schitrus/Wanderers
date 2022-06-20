/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the class for representation of a solar.                *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/solar.h"

/* External Includes */
#include "glm/ext.hpp"

/* Internal Includes */
#include "simulation/object/model/icosahedron.h"

namespace wanderers {
namespace simulation {
namespace object {

Solar::Solar(float radius, float temperature) 
    : Solar{ AstronomicalObject{ AbstractObject{kOrigo, kUp, kFace, glm::vec3{radius}},
                                 new AggregateObject{Object{model::getIcosahedron()}}}, temperature } {}

Solar::Solar(AstronomicalObject astronomical_object, float temperature) 
    : AstronomicalObject{ astronomical_object }, temperature_{ temperature } {}

/*
 * Solar getColor:
 * - Approximates rgb values of the black body radiation that is dependant on the temperature.
 *   Based on the the Stefan–Boltzmann law.
 */
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

void Solar::setTemperature(float temperature) {
    temperature_ = temperature;
}

float Solar::getTemperature() {
    return temperature_;
}

} // namespace object
} // namespace simulation
} // namespace wanderers
