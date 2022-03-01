#include "simulation/object/orbital_system.h"

OrbitalSystem::OrbitalSystem(AstronomicalObject* orbitee) : orbitee_{orbitee},
                                                            orbits_{}, is_paused{false} {}

void OrbitalSystem::addOrbit(Orbit* orbit) { orbits_.push_back(orbit); }

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

OrbitalSystem::~OrbitalSystem() {
	delete orbitee_;
	for (Orbit* orbit : orbits_)
		delete orbit;
}