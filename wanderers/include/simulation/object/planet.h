/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Class for representation of a planet.                                     *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_PLANET_H_
#define WANDERERS_SIMULATION_OBJECT_PLANET_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes */
#include "simulation/object/astronomical_object.h"

namespace wanderers {
namespace simulation {
namespace object {

/*
 * This class represents a planet with surface color.
 * The planet is seen as an astronomical object.
 */
class Planet : public AstronomicalObject {
public:
	Planet();

	Planet(glm::vec3 surface_color, float radius, 
		   float angular_velocity, glm::vec3 rotational_axis, float rotational_angle);

	Planet(model::Mesh* surface, glm::vec3 surface_color, float radius,
		   float angular_velocity, glm::vec3 rotational_axis, float rotational_angle);

	/* Get the surface color of the planet. */
	glm::vec3 getColor();

	/* Returns the matrix describing the planets current rotation and size. */
	glm::mat4 getPlanetMatrix();

	/* Advance the simulation. */
	void elapseTime(double seconds);

private:
	glm::vec3 surface_color_;

	/* The time in seconds it takes for the planet to make one revolution around its axis. */
	float angular_velocity_;
	glm::vec3 rotational_axis_;
	/* Current rotation of the planet as an angle in degrees. */
	float rotational_angle_;

	static constexpr glm::vec3 kDefaultSurfaceColor{ 1.0f, 1.0f, 1.0f };
	static constexpr float kDefaultRadius{ 0.25f };
	static constexpr float kDefaultAngularVelocity{ 60.0f };
	static constexpr glm::vec3 kDefaultRotationalAxis{ 0.0f, 1.0f, 0.0f };
	static constexpr float kDefaultStartingRotationalAngle{ 0.0f };
};

} // namespace simulation
} // namespace object
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_PLANET_H_
