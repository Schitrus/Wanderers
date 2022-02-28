#ifndef ORBITAL_SYSTEM_H
#define ORBITAL_SYSTEM_H

#include <vector>

#include "AstronomicalObject.h"
#include "Orbit.h"

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

#endif // ORBITAL_SYSTEM_H
