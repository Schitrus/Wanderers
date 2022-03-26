/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Class for representation of a solar.                                      *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_SOLAR_H_
#define WANDERERS_SIMULATION_OBJECT_SOLAR_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes */
#include "simulation/object/astronomical_object.h"

namespace wanderers {
namespace simulation {
namespace object {

/*
 * This class represents a solar with temperature that decides the surface color.
 * The solar is seen as an astronomical object.
 */
class Solar : public AstronomicalObject{
public:
	Solar();

	Solar(float temperature, float radius,
		  float angular_velocity, glm::vec3 rotational_axis, float rotational_angle);

	Solar(model::Mesh* surface, float temperature, float radius,
		  float angular_velocity, glm::vec3 rotational_axis, float rotational_angle);

	/* Get the surface color of the solar. Depends on the temperature. */
	glm::vec3 getColor();

	/* Returns the matrix describing the solar current rotation and size. */
	glm::mat4 getSolarMatrix();

	/* Returns the matrix describing the solar current rotation. */
	glm::mat4 getRotationalMatrix();

	/* Advance the simulation. */
	void elapseTime(double seconds);

	glm::mat4 getMatrix();

private:
	/* Temperature of the sun in Kelvin. */
	float temperature_;

	/* The time in seconds it takes for the solar to make one revolution around its axis. */
	float angular_velocity_;
	glm::vec3 rotational_axis_;
	/* Current rotation of the solar as an angle in degrees. */
	float rotational_angle_;

	static constexpr float kDefaultTemperature{ 2500.0f };
	static constexpr float kDefaultRadius{ 1.0f };
	static constexpr float kDefaultAngularVelocity{ 0.0f };
	static constexpr glm::vec3 kDefaultRotationalAxis{ 0.0f, 1.0f, 0.0f };
	static constexpr float kDefaultStartingRotationalAngle{ 0.0f };
};

} // namespace simulation
} // namespace object
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_SOLAR_H_
