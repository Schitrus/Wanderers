#include "OrbitalSystem.h"

#include "Solar.h"
#include "Planet.h"

OrbitalSystem::OrbitalSystem(AstronomicalObject* orbitee) : orbitee_{orbitee},
                                                            orbits_{}, is_paused{false} {}

void OrbitalSystem::addOrbit(Orbit* orbit) { orbits_.push_back(orbit); }

void OrbitalSystem::elapseTime(double seconds) {
	if (!is_paused) {
		orbitee_->elapseTime(seconds);
		for (Orbit* orbit : orbits_)
			orbit->elapseTime(seconds);
	}
}

void OrbitalSystem::pause() {
	is_paused = true;
}

void OrbitalSystem::unpause() {
	is_paused = false;
}

bool OrbitalSystem::isPaused() {
	return is_paused;
}

OrbitalSystem::~OrbitalSystem() {
	delete orbitee_;
	for (Orbit* orbit : orbits_)
		delete orbit;
}

OrbitalSystem* generateSolarsystem() {
	float year = 365.0f;
	float speed_modifier = 360.0f * 0.01f;

	Solar* sun = new Solar{ 5800.0f, 1.0f, speed_modifier * year / 27.0f, glm::vec3{0.0f, 1.0f, 0.0f}, 0.0f };
	OrbitalSystem* solar_system = new OrbitalSystem{ sun };

	Planet* mercury = new Planet{ glm::vec3{0.5f, 0.47f, 0.40f}, 0.1, speed_modifier * year / 59.0f, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f };
	Orbit* mercury_orbit = new Orbit{ mercury, 2.0f, speed_modifier * year / 88.0f, glm::vec3{0.0f, 1.0f, 0.0f}, 0.0f };
	solar_system->addOrbit(mercury_orbit);

	Planet* venus = new Planet{ glm::vec3{0.8f, 0.7f, 0.5f}, 0.3, speed_modifier * year / 243.0f, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f };
	Orbit* venus_orbit = new Orbit{ venus, 4.0f, speed_modifier * year / 224.0f, glm::vec3{0.0f, 1.0f, 0.0f}, 0.0f };
	solar_system->addOrbit(venus_orbit);

	Planet* earth = new Planet{ glm::vec3{0.55f, 0.6f, 0.8f}, 0.35, speed_modifier * year / 1.0f, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f };
	OrbitalSystem* earth_system = new OrbitalSystem{ earth };

	Planet* moon = new Planet{ glm::vec3{0.8f, 0.8f, 0.84f}, 0.08f, speed_modifier * year / 27.0f, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f };
	Orbit* moon_orbit = new Orbit{ moon, 1.0f, -speed_modifier * year / 27.0f, glm::vec3{0.0f, 1.0f, 0.0f}, 0.0f };
	earth_system->addOrbit(moon_orbit);

	Orbit* earth_orbit = new Orbit{ earth_system, 7.0f, speed_modifier * year / 365.0f, glm::vec3{0.0f, 1.0f, 0.0f}, 0.0f };
	solar_system->addOrbit(earth_orbit);

	Planet* mars = new Planet{ glm::vec3{0.8f, 0.45f, 0.2f}, 0.16, speed_modifier * year / 1.03f, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f };
	Orbit* mars_orbit = new Orbit{ mars, 10.0f, speed_modifier * year / 687.0f, glm::vec3{0.0f, 1.0f, 0.0f}, 0.0f };
	solar_system->addOrbit(mars_orbit);

	return solar_system;
}