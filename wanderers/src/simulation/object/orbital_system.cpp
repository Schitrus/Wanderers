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
                                                            orbits_{}, is_paused_{false},
	                                                        simulation_speed_{1.0f} {}

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
	if (!is_paused_) {
		orbitee_->elapseTime(seconds * simulation_speed_);
		for (Orbit* orbit : orbits_)
			orbit->elapseTime(seconds * simulation_speed_);
	}
}

void OrbitalSystem::pause() {
	is_paused_ = true;
}

void OrbitalSystem::unpause() {
	is_paused_ = false;
}

bool OrbitalSystem::isPaused() {
	return is_paused_;
}

void OrbitalSystem::setSpeed(double simulation_speed) {
	simulation_speed_ = simulation_speed;
}

double OrbitalSystem::getSpeed() {
	return simulation_speed_;
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
