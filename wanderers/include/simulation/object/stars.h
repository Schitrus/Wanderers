/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Class for representation of the stars in the sky.                         *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_STARS_H_
#define WANDERERS_SIMULATION_OBJECT_STARS_H_

 /* External Includes */
#include "glm/glm.hpp"

/* Internal Includes */
#include "simulation/object/astronomical_object.h"

/* STL Includes */
#include <vector>

namespace wanderers {
namespace simulation {
namespace object {

/*
 * This class represents the stars in the sky.
 * The solar is seen as an astronomical object.
 */
class Stars : public AstronomicalObject {
public:
	Stars(int number_of_stars, float temperature, float size);

	std::vector<glm::vec3>* generateStars(int number_of_stars);

	/* Get the color of the stars. Depends on the temperature. */
	glm::vec3 getColor();
	
	float getSize();

	/* Advance the simulation. */
	void elapseTime(double seconds);

private:
	glm::vec3 star_positions_;

	float temperature_;

	float size_;
};

} // namespace simulation
} // namespace object
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_STARS_H_
