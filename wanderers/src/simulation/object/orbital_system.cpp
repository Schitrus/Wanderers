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

OrbitalSystem::OrbitalSystem() : OrbitalSystem{kAbstractAstronomicalObject} {}

OrbitalSystem::OrbitalSystem(AstronomicalObject* center_object) 
	: OrbitalSystem{ kAbstractAstronomicalObject, center_object } {}

OrbitalSystem::OrbitalSystem(AstronomicalObject astronomical_object) 
	: AstronomicalObject{ astronomical_object } {}

OrbitalSystem::OrbitalSystem(AstronomicalObject astronomical_object, AstronomicalObject* center_object)
	: OrbitalSystem{astronomical_object} {
	addOrbit(center_object);
}

std::vector<std::pair<AstronomicalObject*, Orbit*>> OrbitalSystem::getOrbits() {
	return orbits_;
}

void OrbitalSystem::addOrbit(AstronomicalObject* object) {
	addOrbit(object, new Orbit{ kNoOrbit });
}

void OrbitalSystem::addOrbit(AstronomicalObject* object, Orbit* orbit) {
	orbit->setParent(this);
	object->setParent(orbit);
	orbits_.push_back(std::make_pair(object, orbit)); 
}

/*
 * OrbitalSystem elapseTime:
 * - If simulation is not paused:
 *   - Elapse time for all orbits.
 */
void OrbitalSystem::elapseTime(double seconds) {
	std::for_each(orbits_.begin(), orbits_.end(), 
		[seconds](std::pair<AstronomicalObject*, Orbit*> current_orbit) {
			current_orbit.first->elapseTime(seconds); 
			current_orbit.second->elapseTime(seconds);  
		});
}

} // namespace object
} // namespace simulation
} // namespace wanderers
