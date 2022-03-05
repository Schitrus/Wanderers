/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Class for representation of an orbital system.      *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/orbital_system.h"

namespace wanderers {
namespace simulation {
namespace object {

OrbitalSystem::OrbitalSystem(AstronomicalObject* orbitee) : orbitee_{orbitee},
                                                            orbits_{}, is_paused{false} {}

AstronomicalObject* OrbitalSystem::getOrbitee() {
	return orbitee_;
}

std::vector<Orbit*> OrbitalSystem::getOrbits() {
	return orbits_;
}

void OrbitalSystem::addOrbit(Orbit* orbit) { orbits_.push_back(orbit); }

/*
 * OrbitalSystem elapseTime:
 * - If simulation is not paused:
 *   - Elapse time for the orbitee.
 *   - Elapse time for all orbits.
 */
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

/*
 * OrbitalSystem Destructor:
 * - Destroy Orbitee.
 * - Destroy all orbits.
 */
OrbitalSystem::~OrbitalSystem() {
	delete orbitee_;
	for (Orbit* orbit : orbits_)
		delete orbit;
}

} // namespace simulation
} // namespace object
} // namespace wanderers
