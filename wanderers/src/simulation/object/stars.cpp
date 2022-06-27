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
#include "glm/gtx/vector_angle.hpp"

/* STL Includes */
#include <random>
#include <chrono>
#include <iostream>

namespace wanderers {
namespace simulation {
namespace object {

/*
 * Stars getColor:
 * - Approximates rgb values of the black body radiation that is dependant on the temperature.
 *   Based on the the Stefan–Boltzmann law.
 */
glm::vec3 temperatureToRGB(float temperature) {
    glm::vec3 color;
    if (temperature <= 6600) {
        color.r = 1.0;
        color.g = glm::clamp((99.4708025861 * log(temperature / 100.0) - 161.1195681661) / 255.0, 0.0, 1.0);
        if (temperature <= 1900) {
            color.b = 0.0;
        }
        else {
            color.b = glm::clamp((138.5177312231 * log(temperature / 100 - 10.0) - 305.0447927307) / 255.0, 0.0, 1.0);
        }
    }
    else {
        color.r = glm::clamp((329.698727446 * pow(temperature / 100.0 - 60.0, -0.1332047592)) / 255.0, 0.0, 1.0);
        color.g = glm::clamp((288.1221695283 * pow(temperature / 100.0 - 60.0, -0.0755148492)) / 255.0, 0.0, 1.0);
        color.b = 1.0;
    }
    return color;
}

/* Random generator with seed set as the time of execution. */
static std::default_random_engine randomizer(std::chrono::system_clock::now().time_since_epoch().count());

Stars::Stars(float temperature, float size, float distance, model::Points* points) 
    : AstronomicalObject{ kDefaultObject, new AggregateObject{ new Object{points} } },
      temperature_{ temperature }, size_{ size }, distance_{distance} {}

// TODO: Move generation of stars


glm::vec3 Stars::generateRandomDirection() {
    std::uniform_real_distribution<float> angle(0.0f, 360.0f);
    std::uniform_real_distribution<float> up(-1.0f, 1.0f);

    float theta{ angle(randomizer) };
    float z{ up(randomizer) };
    return glm::normalize(glm::vec3{ sqrt(1 - z * z) * cos(glm::radians(theta)), sqrt(1 - z * z) * sin(glm::radians(theta)), z });
}

/*
 * Stars generateStars.
 * - Foreach star to be generated.
 *   - Generate a random direction.
 *   - Set star to the random direction times a random distance.
 * - Return the stars.
 */
model::Points* Stars::generateStars(int number_of_stars, float max_distance, glm::vec3 position) {
	std::vector<glm::vec3>* stars = new std::vector<glm::vec3>(number_of_stars);
    std::vector<glm::vec3>* star_color = new std::vector<glm::vec3>(number_of_stars);

	std::uniform_real_distribution<float> angle(0.0f, 360.0f);
	std::uniform_real_distribution<float> up(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distance(1.0f, max_distance);
    std::uniform_real_distribution<float> temperature(4000.0f, 10000.0f);

	for (int i = 0; i < number_of_stars; i++) {
        glm::vec3 direction = generateRandomDirection();
        stars->at(i) = direction * distance(randomizer) + position;
        star_color->at(i) = temperatureToRGB(temperature(randomizer));
	}
    return new model::Points{ stars, star_color };
}

/*
 * Stars generateStars.
 * - Foreach star to be generated.
 *   - Generate a random direction.
 *   - Focus the direction to a disc shape.
 *   - Set star to the random direction.
 * - Return the stars.
 */
model::Points* Stars::generateGalaxyDisc(int number_of_stars, float max_distance, glm::vec3 axis, glm::vec3 position) {
    std::vector<glm::vec3>* star_position = new std::vector<glm::vec3>(number_of_stars);
    std::vector<glm::vec3>* star_color = new std::vector<glm::vec3>(number_of_stars);
    std::uniform_real_distribution<float> angle(0.0f, 1.0f);
    std::uniform_real_distribution<float> up(0.0f, 1.0f);
    std::uniform_real_distribution<float> sign(0.0f, 1.0f);
    std::uniform_real_distribution<float> distance(0.0f, 1.0f);
    std::uniform_real_distribution<float> temperature(4000.0f, 10000.0f);
    int core_depth = 5;
    for (int i = 0; i < number_of_stars; i++) {
        float theta{ angle(randomizer) };
        float z{ up(randomizer) };
        float s{ sign(randomizer) > 0.5f ? 1.0f : -1.0f };
        float st{ sign(randomizer) > 0.5f ? 1.0f : -1.0f };
        float current_core_depth = (i % core_depth) + 1;
        //stars->at(i) = glm::vec3{ sqrt(1 - z * z) * cos(glm::radians(st * (1 - pow(theta, 1.0f/current_core_depth)) * 180.0f)), 
        //                          s * (1 - 0.5f * current_core_depth / core_depth) * (0.1f * z + 0.7f * pow(z, 2.0f) + 0.2f * pow(z, 4.0f)), 
        //                          sqrt(1 - z * z) * sin(glm::radians(st * (1 - pow(theta, 1.0f / current_core_depth)) * 180.0f)) } * distance(randomizer);

        glm::vec3 dir = generateRandomDirection();
        dir = glm::normalize(dir);
        dir.y = 0.5f * dir.y;

        //float dis = distance(randomizer);
        //dis = pow(dis, 255.0f);
        //if (dis < 0.00001f)
        //    continue;
        //
        //float h_param = pow(0.5f + pow(0.5f * (1 + cos(glm::radians(180.0f * dis))), 8.0f) + 0.5f * (1 - dis), 1.5f);
        //
        //std::uniform_real_distribution<float> height(-h_param, h_param);
        //
        //float height_factor = distance(randomizer);
        //height_factor = pow(height_factor, 5.0f);
        //float h = height(randomizer);
        //dir = dir * dis * max_distance;
        //dir.y = 0.1f * h * height_factor * max_distance;

        float dist = 1.0f/cbrt(1.0f-distance(randomizer)) - 1.0f;
        float up_dist = distance(randomizer);
        up_dist = 0.3f/sqrt(1.0f - up_dist) + 0.7f * up_dist - 0.3f;

        dir.y *= 1.0f / (pow(2.0f * dist, 2.0f) + 1.0f);
        dir.y += 0.1f * (distance(randomizer) >= 0.5f ? 1.0f : -1.0f) * std::max(0.0f, 1.0f / (pow(0.1f * dist, 2.0f) + 1.0f) * up_dist - 1.0f) ;


        if (dist > 1.0f) {
            dist = log(dist) + 1.0f;
        }

        glm::vec3 star_pos = dir * max_distance * dist;

        star_pos = glm::rotation(glm::vec3{ 0.0, 1.0f, 0.0f }, glm::normalize(axis)) * glm::vec4{star_pos, 1.0f};

        star_position->at(i) = star_pos + position;
        star_color->at(i) = temperatureToRGB(temperature(randomizer));
    }

    return new model::Points{ star_position, star_color };
}

/*
 * Stars generateStars.
 * - Foreach star to be generated.
 *   - Generate deviation from the center vector whitin distance angle.
 *   - Set direction of star according to the deviation.
 *   - Set star to the direction.
 * - Return the stars.
 */
model::Points* Stars::generateCluster(int number_of_stars, float distance_angle, float dis, glm::vec3 center, glm::vec3 position) {
    std::vector<glm::vec3>* stars = new std::vector<glm::vec3>(number_of_stars);
    std::uniform_real_distribution<float> angle(0.0f, 360.0f);
    std::uniform_real_distribution<float> distance(0.0f, 1.0f);
    std::uniform_real_distribution<float> temperature(4000.0f, 10000.0f);

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
        glm::vec3 v = generateRandomDirection() * sin(glm::radians(distance_angle)) * distance(randomizer);
        stars->at(i) = (center + v) * dis + position;
    }
    return new model::Points{ stars };
}

void Stars::elapseTime(double seconds) { }

glm::vec3 Stars::getColor() {
    return temperatureToRGB(temperature_);
}

float Stars::getSize() { return size_; }

float Stars::getDistance() { return distance_; }

} // namespace object
} // namespace simulation
} // namespace wanderers
