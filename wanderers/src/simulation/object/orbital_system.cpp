/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the class for representation of an orbital system.      *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/orbital_system.h"

namespace wanderers {
namespace simulation {
namespace object {

	OrbitalSystem::OrbitalSystem(AstronomicalObject* orbitee) : AstronomicalObject{0}, orbitee_ { orbitee },
                                                                orbits_{} {}

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
	orbitee_->elapseTime(seconds);
	for (Orbit* orbit : orbits_)
		orbit->elapseTime(seconds);
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

} // namespace object
} // namespace simulation
} // namespace wanderers
