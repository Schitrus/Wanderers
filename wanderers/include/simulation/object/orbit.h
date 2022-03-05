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
class Orbit {
public:
	Orbit(AstronomicalObject* object);

	Orbit(AstronomicalObject* object, float radius, float angular_velocity, glm::vec3 orbital_axis, float orbital_angle);

	~Orbit();

	AstronomicalObject* getOrbitor();

	/* Returns the matrix describing the object in it's current place in orbit. */
	glm::mat4 getOrbitMatrix();

	/* Advance the simulation. */
	void elapseTime(double seconds);

private:
	/* The object in orbit. */
	AstronomicalObject* orbitor_;

	/* The radius of the orbit. */
	float radius_;

	/* The time in seconds it takes for the object to make one revolution. */
	float angular_velocity_;
	glm::vec3 orbital_axis_;
	/* Current position in the orbit as an angle in degrees. */
	float orbital_angle_;

	static constexpr float kDefaultRadius{ 4.0f };
	static constexpr float kDefaultAngularVelocity{ 10.0f };
	static constexpr glm::vec3 kDefaultOrbitalAxis{0.0f, 1.0f, 0.0f};
	static constexpr float kDefaultStartingOrbitalAngle{ 0.0f };
};

} // namespace simulation
} // namespace object
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_ORBIT_H_