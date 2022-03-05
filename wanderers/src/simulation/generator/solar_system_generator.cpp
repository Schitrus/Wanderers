/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of functions for generating a Solarsystem and parts of it. *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/generator/solar_system_generator.h"

/* External Includes */
#include <glm/ext.hpp>

/* STL Includes */
#include <random>
#include <chrono>

namespace wanderers {
namespace simulation {
namespace generator {

/* Random generator with seed set as the time of execution. */
static std::default_random_engine randomizer( std::chrono::system_clock::now().time_since_epoch().count());

float calcRotationalAxisAngle(float distr) {
	return 170.0f * pow(distr, 80.0f) + 10.0f * pow(distr, 2.0f);
}

float calcOrbitalAxisAngle(float distr) {
	return 179.0f * pow(distr, 160.0f) + 1.0f * pow(distr, 2.0f);
}

glm::vec3 generateRotationalAxis() {
	std::uniform_real_distribution<float> axis(0.0f, 1.0f);
	return glm::vec3{ glm::rotate(glm::mat4{1.0f}, glm::radians(calcRotationalAxisAngle(axis(randomizer))), glm::vec3{1.0f, 0.0f, 0.0f})
					* glm::rotate(glm::mat4{1.0f}, glm::radians(calcRotationalAxisAngle(axis(randomizer))), glm::vec3{0.0f, 0.0f, 1.0f})
					* glm::vec4{0.0f, 1.0f, 0.0f, 0.0f} };
}

glm::vec3 generateOrbitalAxis() {
	std::uniform_real_distribution<float> axis(0.0f, 1.0f);
	return glm::vec3{ glm::rotate(glm::mat4{1.0f}, glm::radians(calcOrbitalAxisAngle(axis(randomizer))), glm::vec3{1.0f, 0.0f, 0.0f})
					* glm::rotate(glm::mat4{1.0f}, glm::radians(calcOrbitalAxisAngle(axis(randomizer))), glm::vec3{0.0f, 0.0f, 1.0f})
					* glm::vec4{0.0f, 1.0f, 0.0f, 0.0f} };
}

/*
 * generateSolarSystem:
 * - Generate solar with a radius between 0.5 and 4.0 .
 * - First planet system set at a distance twice the radius of the solar.
 * - Until at the edge of the solar system:
 *   - Generate planet system with a radius of between 25% and 200% that of the solar radius.
 *   - Add planet system to the solar system at the current distance.
 *   - increment the distance as big as the planet system.
 * - Return solar system.
 */
object::OrbitalSystem* generateSolarSystem(float radius) {
	std::uniform_real_distribution<float> solar_size(0.5f, 4.0f);
	float solar_radius{ solar_size(randomizer) };
	object::OrbitalSystem* solar_system = new object::OrbitalSystem{ generateSolar(solar_radius) };
	float distance = 2.0f * solar_radius;
	while (radius >= distance) {
		std::uniform_real_distribution<float> planet_system_size(0.25f*solar_radius, 2.0f*solar_radius);
		float planet_system_radius{ std::min(planet_system_size(randomizer), radius/2.0f) };
		distance += planet_system_radius;
		solar_system->addOrbit(generatePlanetSystem(planet_system_radius, distance));
		distance += planet_system_radius;
	}
	return solar_system;
}

/*
 * generateSolar:
 * - Randomize the temperature between 1,000K and 10,000K.
 * - Set the given radius.
 * - Randomize the spin of the solar.
 * - generateAxis.
 */
object::Solar* generateSolar(float radius) {
	std::uniform_real_distribution<float> temperature(1000.0f, 10000.0f);
	std::uniform_real_distribution<float> spin(-360.0f, 360.0f);
	return new object::Solar{ temperature(randomizer), radius, spin(randomizer), generateRotationalAxis(), spin(randomizer) };
}

/*
 * generatePlanetSystem:
 * - Generate planet with a radius between 10% and 35% that of the planet system radius.
 * - Create planet system with the planet as orbitee.
 * - Randomize number of moons:
 *   - Set moon radius as a fraction of remaining size of the planet system.
 *   - Generate moon.
 *   - Make orbit and add it to the planet system.
 *   - Increase distance from moon.
 * - Set the given orbit radius.
 * - Set the orbit speed as dependent to the sqrt of the orbit radius.
 * - Rotation set around the y axis.
 * - Start position around the orbit is randomized.
 */
object::Orbit* generatePlanetSystem(float radius, float orbit_radius) {
	std::uniform_real_distribution<float> planet_size(0.1f, 0.35f);
	std::uniform_real_distribution<float> start_pos(0.0f, 360.0f);
	
	float planet_radius{ radius * planet_size(randomizer) };
	object::OrbitalSystem* planet_system = new object::OrbitalSystem{ generatePlanet(planet_radius) };

	float moon_distance{ planet_radius * 2.0f };
	std::uniform_int_distribution<int> moon_distr(0, static_cast<int>((radius - moon_distance) / 0.2f));
	int num_of_moons{ std::min(moon_distr(randomizer), 6) };
	
	for (int i = 0; i < num_of_moons; i++) {
		std::uniform_real_distribution<float> moon_size(0.8f/(num_of_moons - i), 1.2f / (num_of_moons - i));
		float moon_radius{ std::min(0.3f * (radius - moon_distance) * moon_size(randomizer), 0.7f * planet_radius)};
		object::Planet* moon{ generatePlanet(moon_radius) };
		planet_system->addOrbit(new object::Orbit{ moon, moon_distance, 180.0f / sqrt(moon_distance), generateOrbitalAxis(), start_pos(randomizer) });
		moon_distance += 2 * moon_radius + (radius - moon_distance) * moon_size(randomizer);
	}

	return new object::Orbit{ planet_system, orbit_radius, 18.0f/sqrt(orbit_radius), generateOrbitalAxis(), start_pos(randomizer) };;
}

/*
 * generatePlanet:
 * - Randomize surface color.
 * - Set the given radius.
 * - Randomize the spin of the planet.
 * - generateAxis.
 */
object::Planet* generatePlanet(float radius) {
	std::uniform_real_distribution<float> spin(-360.0f, 360.0f);
	std::uniform_real_distribution<float> color(0.0f, 1.0f);
	std::uniform_real_distribution<float> axis(0.0f, 1.0f);
	return new object::Planet{ glm::vec3{color(randomizer), color(randomizer), color(randomizer)}, radius, spin(randomizer), generateRotationalAxis(), spin(randomizer) };
}

} // namespace generator
} // namespace simulation
} // namespace wanderers
