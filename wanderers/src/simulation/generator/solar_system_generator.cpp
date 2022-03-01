#include "simulation/generator/solar_system_generator.h"

#include <random>
#include <chrono>

static std::default_random_engine randomizer( std::chrono::system_clock::now().time_since_epoch().count());

OrbitalSystem* generateSolarSystem(float radius) {
	std::uniform_real_distribution<float> solar_size(0.5f, 4.0f);
	float solar_radius{ solar_size(randomizer) };
	OrbitalSystem* solar_system = new OrbitalSystem{ generateSolar(solar_radius) };
	float distance = 2.0f * solar_radius;
	while (radius >= distance) {
		std::uniform_real_distribution<float> planet_system_size(0.5f, 2.0f*solar_radius);
		float planet_system_radius{ std::min(planet_system_size(randomizer), radius/2.0f) };
		distance += planet_system_radius;
		solar_system->addOrbit(generatePlanetSystem(planet_system_radius, distance));
		distance += planet_system_radius;
	}
	return solar_system;
}

Solar* generateSolar(float radius) {
	std::uniform_real_distribution<float> temperature(1000.0f, 10000.0f);
	std::uniform_real_distribution<float> spin(-360.0f, 360.0f);
	return new Solar{ temperature(randomizer), radius, spin(randomizer), glm::vec3{0.0f, 1.0f, 0.0f}, 0.0f };
}

Orbit* generatePlanetSystem(float radius, float orbit_radius) {
	std::uniform_real_distribution<float> planet_size(radius*0.1f, radius*0.5f);
	std::uniform_real_distribution<float> start_pos(0.0f, 360.0f);
	return new Orbit{ generatePlanet(planet_size(randomizer)), orbit_radius, 18.0f/sqrt(orbit_radius), glm::vec3{0.0f, 1.0f, 0.0f}, start_pos(randomizer) };;
}

Planet* generatePlanet(float radius) {
	std::uniform_real_distribution<float> spin(-360.0f, 360.0f);
	std::uniform_real_distribution<float> color(0.0f, 1.0f);
	return new Planet{ glm::vec3{color(randomizer), color(randomizer), color(randomizer)}, radius, spin(randomizer), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f };
}
