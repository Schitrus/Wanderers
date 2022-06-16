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
class Orbit : public AstronomicalObject {
public:	
	Orbit(float eccentricity, float semimajor_axis, float inclination, float longitude_of_acending_node, float argument_of_periapsis, float true_anomaly, float angular_velocity);

	Orbit(float radius, float angular_velocity, float orbital_angle);
	Orbit(float major_axis, float minor_axis, float angular_velocity, float orbital_angle);

	Orbit(float radius, float angular_velocity, float orbital_angle, glm::vec3 orbital_axis, glm::vec3 orbital_face);
	Orbit(float major_axis, float minor_axis, float angular_velocity, float orbital_angle, glm::vec3 orbital_axis, glm::vec3 orbital_face);

	Orbit(AstronomicalObject astronomical_object, float major_axis, float minor_axis, float angular_velocity, float orbital_angle, glm::vec3 orbital_axis, glm::vec3 orbital_face);

	Orbit(const Orbit& orbit) = default;

	void setMajorAxis(float major_axis);
	float getMajorAxis();

	void setMinorAxis(float minor_axis);
	float getMinorAxis();

	void setAngularVelocity(float angular_velocity);
	float getAngularVelocity();

	void setOrbitalAngle(float orbital_angle);
	float getOrbitalAngle();

	void setOrbitalAxis(glm::vec3 orbital_axis);
	glm::vec3 getOrbitalAxis();

	void setOrbitalFace(glm::vec3 orbital_face);
	glm::vec3 getOrbitalFace();

	void setOrbitalSide(glm::vec3 orbital_side);
	glm::vec3 getOrbitalSide();

	/* Returns the matrix describing the object in it's current place in orbit. */
	glm::mat4 getOrbitMatrix();

	virtual glm::mat4 getMatrix();

	/* Advance the simulation. */
	virtual void elapseTime(double seconds);

private:
	float major_axis_;
	float minor_axis_;

	/* Current position in the orbit as an angle in degrees. */
	float angular_velocity_;
	float orbital_angle_;

	/* The time in seconds it takes for the object to make one revolution. */
	glm::vec3 orbital_axis_;
	glm::vec3 orbital_face_;
	glm::vec3 orbital_side_;
};

static const Orbit kNoOrbit{0.0f, 0.0f, 0.0f};

} // namespace simulation
} // namespace object
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_ORBIT_H_