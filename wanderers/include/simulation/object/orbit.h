/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Class for representation of an astronomical object in orbit.              *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_ORBIT_H_
#define WANDERERS_SIMULATION_OBJECT_ORBIT_H_

/* Internal Includes */
#include "simulation/object/astronomical_object.h"

namespace wanderers {
namespace simulation {
namespace object {

/*
 * This class represents an orbit of an astronimical object.
 * The object the orbitor orbits is irrelevant.
 */
class Orbit : public AbstractObject { // MAKE CHILD OF ABSTRACT OBJECT
public:

	Orbit(float radius, glm::vec3 orbital_axis, float orbital_angle, float angular_velocity);

	Orbit(const Orbit& orbit) = default;

	glm::vec3 getOrbitalAxis();

	float getAngle();

	/* Returns the matrix describing the object in it's current place in orbit. */
	glm::mat4 getOrbitMatrix();

	virtual glm::mat4 getMatrix();

	/* Advance the simulation. */
	virtual void elapseTime(double seconds);

private:
	/* The time in seconds it takes for the object to make one revolution. */
	glm::vec3 orbital_axis_; 
	/* Current position in the orbit as an angle in degrees. */
	float orbital_angle_;
	float angular_velocity_;
};

} // namespace simulation
} // namespace object
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_ORBIT_H_