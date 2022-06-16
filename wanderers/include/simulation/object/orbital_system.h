/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Class for representation of an orbital system.                            *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_ORBITAL_SYSTEM_H_
#define WANDERERS_SIMULATION_OBJECT_ORBITAL_SYSTEM_H_

/* Internal Includes */
#include "simulation/object/astronomical_object.h"
#include "simulation/object/orbit.h"

/* STL Includes */
#include <vector>

namespace wanderers {
namespace simulation {
namespace object {

/*
 * This class is a representation of an orbital system with an orbitee in the center of the system
 *  and orbits around it.
 * The orbital system is seen as an astronomical object.
 */
class OrbitalSystem : public AstronomicalObject {
public:
	OrbitalSystem();
	OrbitalSystem(AstronomicalObject* center_object);

	OrbitalSystem(AstronomicalObject astronomical_object);
	OrbitalSystem(AstronomicalObject astronomical_object, AstronomicalObject* center_object);

	std::vector<std::pair<AstronomicalObject*, Orbit*>> getOrbits();

	/* Add and orbit to the orbital system. */
	void addOrbit(AstronomicalObject* object, Orbit* orbit);
	void addOrbit(AstronomicalObject* object);

	/* Advance the simulation. */
	void elapseTime(double seconds);

private:
	std::vector<std::pair<AstronomicalObject*, Orbit*>> orbits_;
};

} // namespace simulation
} // namespace object
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_ORBITAL_SYSTEM_H_
