/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the class for representation of an orbital system.      *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/orbital_system.h"

#include <iostream>
#include <algorithm>

namespace wanderers {
namespace simulation {
namespace object {

OrbitalSystem::OrbitalSystem(AbstractObject abstract_object) 
	: AbstractObject{ abstract_object } {}

std::vector<std::pair<AbstractObject*, Orbit*>> OrbitalSystem::getOrbits() {
	return orbits_;
}

void OrbitalSystem::addOrbit(AbstractObject* object, Orbit* orbit) { orbits_.push_back(std::make_pair(object, orbit)); }

/*
 * OrbitalSystem elapseTime:
 * - If simulation is not paused:
 *   - Elapse time for the orbitee.
 *   - Elapse time for all orbits.
 */
void OrbitalSystem::elapseTime(double seconds) {
	std::for_each(orbits_.begin(), orbits_.end(), 
		[seconds](std::pair<AbstractObject*, Orbit*> current_orbit) { 
			current_orbit.first->elapseTime(seconds); 
			current_orbit.second->elapseTime(seconds);  
		});
}

} // namespace object
} // namespace simulation
} // namespace wanderers
