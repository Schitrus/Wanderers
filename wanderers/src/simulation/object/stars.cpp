/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the class for representation of the stars in the sky.   *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/stars.h"

/* External Includes */
#include "glm/ext.hpp"

/* Internal Includes */
#include "simulation/object/model/Points.h"

/* STL Includes */
#include <random>
#include <chrono>

namespace wanderers {
namespace simulation {
namespace object {

/* Random generator with seed set as the time of execution. */
static std::default_random_engine randomizer(std::chrono::system_clock::now().time_since_epoch().count());

Stars::Stars(int number_of_stars, float temperature, float size) : AstronomicalObject{ new model::Points{generateStars(number_of_stars)} },
                                                                   temperature_{ temperature }, size_{ size } {}

// TODO: Move generation of stars

std::vector<glm::vec3>* Stars::generateStars(int number_of_stars) {
	std::vector<glm::vec3>* stars = new std::vector<glm::vec3>(number_of_stars);
	std::uniform_real_distribution<float> angle(0.0f, 360.0f);
	std::uniform_real_distribution<float> up(-1.0f, 1.0f);
	for (int i = 0; i < number_of_stars; i++) {
		float theta{ angle(randomizer) };
		float z{ up(randomizer) };
		stars->at(i) = glm::vec3{sqrt(1-z*z) * cos(glm::radians(theta)), sqrt(1 - z * z) * sin(glm::radians(theta)), z };
	}
	return stars;
}

void Stars::elapseTime(double seconds) { }

/*
 * Stars getColor:
 * - Approximates rgb values of the black body radiation that is dependant on the temperature.
 *   Based on the the Stefan–Boltzmann law.
 */
glm::vec3 Stars::getColor() {
    glm::vec3 color;
    if (temperature_ <= 6600) {
        color.r = 1.0;
        color.g = glm::clamp((99.4708025861 * log(temperature_ / 100.0) - 161.1195681661) / 255.0, 0.0, 1.0);
        if (temperature_ <= 1900) {
            color.b = 0.0;
        }
        else {
            color.b = glm::clamp((138.5177312231 * log(temperature_ / 100 - 10.0) - 305.0447927307) / 255.0, 0.0, 1.0);
        }
    }
    else {
        color.r = glm::clamp((329.698727446 * pow(temperature_ / 100.0 - 60.0, -0.1332047592)) / 255.0, 0.0, 1.0);
        color.g = glm::clamp((288.1221695283 * pow(temperature_ / 100.0 - 60.0, -0.0755148492)) / 255.0, 0.0, 1.0);
        color.b = 1.0;
    }
    return color;
}

float Stars::getSize() { return size_; }

} // namespace object
} // namespace simulation
} // namespace wanderers
