/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of functions for generating a Solarsystem and parts of it. *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/generator/solar_system_generator.h"

/* STL Includes */
#include <random>
#include <chrono>

namespace wanderers {
namespace simulation {
namespace generator {

/* Random generator with seed set as the time of execution. */
static std::default_random_engine randomizer( std::chrono::system_clock::now().time_since_epoch().count());

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
 * - Rotation set around the y axis.
 */
object::Solar* generateSolar(float radius) {
	std::uniform_real_distribution<float> temperature(1000.0f, 10000.0f);
	std::uniform_real_distribution<float> spin(-360.0f, 360.0f);
	return new object::Solar{ temperature(randomizer), radius, spin(randomizer), glm::vec3{0.0f, 1.0f, 0.0f}, 0.0f };
}

/*
 * generatePlanetSystem:
 * - Generate planet with a radius between 10% and 50% that of the planet system radius.
 * - Set the given orbit radius.
 * - Set the orbit speed as dependent to the sqrt of the orbit radius.
 * - Rotation set around the y axis.
 * - Start position around the orbit is randomized.
 */
object::Orbit* generatePlanetSystem(float radius, float orbit_radius) {
	std::uniform_real_distribution<float> planet_size(radius*0.1f, radius*0.5f);
	std::uniform_real_distribution<float> start_pos(0.0f, 360.0f);
	return new object::Orbit{ generatePlanet(planet_size(randomizer)), orbit_radius, 18.0f/sqrt(orbit_radius), glm::vec3{0.0f, 1.0f, 0.0f}, start_pos(randomizer) };;
}

/*
 * generatePlanet:
 * - Randomize surface color.
 * - Set the given radius.
 * - Randomize the spin of the planet.
 * - Rotation set around the y axis.
 */
object::Planet* generatePlanet(float radius) {
	std::uniform_real_distribution<float> spin(-360.0f, 360.0f);
	std::uniform_real_distribution<float> color(0.0f, 1.0f);
	return new object::Planet{ glm::vec3{color(randomizer), color(randomizer), color(randomizer)}, radius, spin(randomizer), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f };
}

} // namespace generator
} // namespace simulation
} // namespace wanderers
