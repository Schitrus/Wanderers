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
	Planet(AstronomicalObject astronomical_object, glm::vec3 surface_color);

	/* Get the surface color of the planet. */
	glm::vec3 getColor();

private:
	glm::vec3 surface_color_;
};

} // namespace simulation
} // namespace object
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_PLANET_H_
