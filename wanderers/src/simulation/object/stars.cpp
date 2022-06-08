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
#include "glm/gtx/rotate_vector.hpp"

/* STL Includes */
#include <random>
#include <chrono>

namespace wanderers {
namespace simulation {
namespace object {

/* Random generator with seed set as the time of execution. */
static std::default_random_engine randomizer(std::chrono::system_clock::now().time_since_epoch().count());

Stars::Stars(float temperature, float size, float distance, model::Points* points) 
    : AstronomicalObject{ AbstractObject{glm::vec3{0.0f}, kUp, kFace, 1.0f},
      new Object{ AbstractObject{glm::vec3{0.0f}, kUp, kFace, 1.0f}, points},
      kUp, 0.0f, 0.0f },
    temperature_{ temperature }, size_{ size }, distance_{distance} { }

// TODO: Move generation of stars

model::Points* Stars::generateStars(int number_of_stars, float max_distance) {
	std::vector<glm::vec3>* stars = new std::vector<glm::vec3>(number_of_stars);
	std::uniform_real_distribution<float> angle(0.0f, 360.0f);
	std::uniform_real_distribution<float> up(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distance(1.0f, max_distance);
	for (int i = 0; i < number_of_stars; i++) {
		float theta{ angle(randomizer) };
		float z{ up(randomizer) };
        glm::vec3 direction = glm::normalize(glm::vec3{ sqrt(1 - z * z) * cos(glm::radians(theta)), sqrt(1 - z * z) * sin(glm::radians(theta)), z });
        stars->at(i) = direction * distance(randomizer);
	}
    return new model::Points{ stars };
}

model::Points* Stars::generateGalaxyDisc(int number_of_stars) {
    std::vector<glm::vec3>* stars = new std::vector<glm::vec3>(number_of_stars);
    std::uniform_real_distribution<float> angle(0.0f, 1.0f);
    std::uniform_real_distribution<float> up(0.0f, 1.0f);
    std::uniform_real_distribution<float> sign(0.0f, 1.0f);
    int core_depth = 5;
    for (int i = 0; i < number_of_stars; i++) {
        float theta{ angle(randomizer) };
        float z{ up(randomizer) };
        float s{ sign(randomizer) > 0.5f ? 1.0f : -1.0f };
        float st{ sign(randomizer) > 0.5f ? 1.0f : -1.0f };
        float current_core_depth = (i % core_depth) + 1;
        stars->at(i) = glm::vec3{ sqrt(1 - z * z) * cos(glm::radians(st * (1 - pow(theta, 1.0f/current_core_depth)) * 180.0f)), 
                                  s * (1 - 0.5f * current_core_depth / core_depth) * (0.1f * z + 0.7f * pow(z, 2.0f) + 0.2f * pow(z, 4.0f)), 
                                  sqrt(1 - z * z) * sin(glm::radians(st * (1 - pow(theta, 1.0f / current_core_depth)) * 180.0f)) };
    }
    return new model::Points{ stars };
}

glm::vec3 Stars::generateRandomDirection() {
    std::uniform_real_distribution<float> angle(0.0f, 360.0f);
    std::uniform_real_distribution<float> up(-1.0f, 1.0f);

    // cluster center
    float theta{ angle(randomizer) };
    float z{ up(randomizer) };
    return glm::normalize(glm::vec3{ sqrt(1 - z * z) * cos(glm::radians(theta)), sqrt(1 - z * z) * sin(glm::radians(theta)), z });
}

model::Points* Stars::generateCluster(int number_of_stars, float distance_angle, glm::vec3 center) {
    std::vector<glm::vec3>* stars = new std::vector<glm::vec3>(number_of_stars);
    std::uniform_real_distribution<float> angle(0.0f, 360.0f);
    std::uniform_real_distribution<float> distance(0.0f, 1.0f);

    for (int i = 0; i < number_of_stars; i++) {
        float theta{ angle(randomizer) };
        float dist{ distance(randomizer) };
        float rot{ angle(randomizer) };
        glm::vec3 normal = glm::normalize(glm::cross(center, 
                                                    (center != glm::vec3{0.0f, 0.0f, 0.0f} ? 
                                                               glm::vec3{ 0.0f, 0.0f, 1.0f } : 
                                                               glm::vec3{ 1.0f, 0.0f, 0.0f })));
        glm::vec3 deviation = glm::normalize(glm::rotate(center, glm::radians(dist * dist * distance_angle), normal));
        deviation = glm::normalize(glm::rotate(deviation, glm::radians(rot), center));
        stars->at(i) = deviation;
    }
    return new model::Points{ stars };
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

float Stars::getDistance() { return distance_; }

} // namespace object
} // namespace simulation
} // namespace wanderers
