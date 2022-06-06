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
	Solar(AstronomicalObject astronomical_object, float temperature);

	/* Get the surface color of the solar. Depends on the temperature. */
	glm::vec3 getColor();

private:
	/* Temperature of the sun in Kelvin. */
	float temperature_;
};

} // namespace simulation
} // namespace object
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_SOLAR_H_
