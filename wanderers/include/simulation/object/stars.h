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
#include "model/points.h"

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
	Stars(float temperature, float size, float distance, model::Points* points);

	static model::Points* generateStars(int number_of_stars, float max_distance);

	static glm::vec3 generateRandomDirection();

	static model::Points* generateCluster(int number_of_stars, float angle, glm::vec3 direction = generateRandomDirection());

	static model::Points* generateGalaxyDisc(int number_of_stars);

	/* Get the color of the stars. Depends on the temperature. */
	glm::vec3 getColor();
	
	float getSize();

	float getDistance();

	/* Advance the simulation. */
	void elapseTime(double seconds);

private:
	float temperature_;

	float distance_;

	float size_;
};

} // namespace simulation
} // namespace object
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_STARS_H_
