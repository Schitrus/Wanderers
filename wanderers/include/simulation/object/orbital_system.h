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

namespace simulation {
namespace object {

/*
 * This class is a representation fo an orbital system with an orbitee in the center of the system
 *  and orbits around it.
 * The orbital system is seen as an astronomical object.
 */
class OrbitalSystem : public AstronomicalObject {
public:
	OrbitalSystem(AstronomicalObject* orbitee);

	~OrbitalSystem();

	AstronomicalObject* getOrbitee();
	std::vector<Orbit*> getOrbits();

	/* Add and orbit to the orbital system. */
	void addOrbit(Orbit* orbit);

	/* Advance the simulation. */
	void elapseTime(double seconds);

	/* Pause/resume the simulation. */
	void pause();
	void unpause();

	bool isPaused();

private:
	AstronomicalObject* orbitee_;
	std::vector<Orbit*> orbits_;

	bool is_paused;
};

} // namespace simulation
} // namespace object

#endif // WANDERERS_SIMULATION_OBJECT_ORBITAL_SYSTEM_H_
