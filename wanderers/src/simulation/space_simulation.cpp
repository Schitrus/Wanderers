/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the class that simulates a solar system.                *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/space_simulation.h"

namespace wanderers {
namespace simulation {

SpaceSimulation::SpaceSimulation() : solar_systems_{},
                                     group_of_stars_{},
	                                 is_paused_{false},
	                                 simulation_speed_{1.0f} {}


void SpaceSimulation::addSolarSystem(object::OrbitalSystem* solar_system) { solar_systems_.push_back(solar_system); }

void SpaceSimulation::addStars(object::Stars* stars) { group_of_stars_.push_back(stars); }

std::vector<object::OrbitalSystem*> SpaceSimulation::getSolarSystems() { return solar_systems_; }

std::vector<object::Stars*> SpaceSimulation::getGroupOfStars() { return group_of_stars_; }

/*
 * SpaceSimulation elapseTime:
 * - If simulation is not paused:
 *   - Elapse time for the orbitee.
 *   - Elapse time for all orbits.
 */
void SpaceSimulation::elapseTime(double seconds) {
	if (!is_paused_) {
		for(object::OrbitalSystem* solar_system : solar_systems_)
		solar_system->elapseTime(seconds * simulation_speed_);
	}
}

void SpaceSimulation::pause() {
	is_paused_ = true;
}

void SpaceSimulation::unpause() {
	is_paused_ = false;
}

bool SpaceSimulation::isPaused() {
	return is_paused_;
}

void SpaceSimulation::setSpeed(double simulation_speed) {
	simulation_speed_ = simulation_speed;
}

double SpaceSimulation::getSpeed() {
	return simulation_speed_;
}

/*
 * SpaceSimulation Destructor:
 * - Destroy solar system.
 * - Destroy stars.
 */
SpaceSimulation::~SpaceSimulation() {
	for(object::OrbitalSystem* solar_system : solar_systems_)
		delete solar_system;
	for (object::Stars* stars : group_of_stars_)
		delete stars;
}

} // namespace simulation
} // namespace wanderers
