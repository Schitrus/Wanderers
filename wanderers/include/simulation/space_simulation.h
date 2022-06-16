/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Class that simulates a solar system.                                      *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_SPACE_SIMULATION_H_
#define WANDERERS_SIMULATION_SPACE_SIMULATION_H_

/* Internal Includes */
#include "simulation/object/orbital_system.h"
#include "simulation/object/stars.h"

#include "render/camera.h"

/* STL Includes */
#include <vector>

namespace wanderers {
namespace simulation {

/*
 * This class simulates a solar system.
 */
class SpaceSimulation {
public:
	SpaceSimulation(object::CameraObject* camera_object);

	~SpaceSimulation();

	/* Add solar system to the simulation. */
	void addSolarSystem(object::OrbitalSystem* solar_system);

	/* Add stars. */
	void addStars(object::Stars* stars);

	std::vector<object::OrbitalSystem*> getSolarSystems();

	std::vector<object::Stars*> getGroupOfStars();

	/* Advance the simulation. */
	void elapseTime(double seconds);

	/* Pause/resume the simulation. */
	void pause();
	void unpause();

	bool isPaused();

	/* Set the speed of the simulation*/
	void setSpeed(double simulation_speed);
	double getSpeed();

	unsigned int getCameraFocusId();

	void setCameraFocusId(unsigned int focus_id);

	unsigned int cycleCameraFocusId();

private:
	std::vector<object::OrbitalSystem*> solar_systems_;
	std::vector<object::Stars*> group_of_stars_;

	object::CameraObject* camera_object_;

	std::vector<object::AstronomicalObject*> astrological_catalog_;

	bool is_paused_;

	double simulation_speed_;

	unsigned int camera_focus_id_;
};

} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_SPACE_SIMULATION_H_
