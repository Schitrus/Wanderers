#ifndef WANDERERS_SIMULATION_OBJECT_ORBITAL_SYSTEM_H_
#define WANDERERS_SIMULATION_OBJECT_ORBITAL_SYSTEM_H_

/* Internal Includes */
#include "simulation/object/astronomical_object.h"
#include "simulation/object/orbit.h"

/* STL Includes */
#include <vector>

class OrbitalSystem : public AstronomicalObject {
public:
	OrbitalSystem(AstronomicalObject* orbitee);

	~OrbitalSystem();

	void addOrbit(Orbit* orbit);

	void elapseTime(double seconds);

	void pause();
	void unpause();

	bool isPaused();

private:
	AstronomicalObject* orbitee_;
	std::vector<Orbit*> orbits_;

	bool is_paused;

	friend class SpaceRenderer;
};

OrbitalSystem* generateSolarsystem();

#endif // WANDERERS_SIMULATION_OBJECT_ORBITAL_SYSTEM_H_
