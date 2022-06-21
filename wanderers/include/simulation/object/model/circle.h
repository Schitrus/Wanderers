/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class generates a circle model.                                      *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_MODEL_CIRCLE_H_
#define WANDERERS_SIMULATION_OBJECT_MODEL_CIRCLE_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes */
#include "simulation/object/model/mesh.h"

/* STL Includes */
#include <vector>

namespace wanderers {
namespace simulation {
namespace object {
namespace model {

/*
 * Class to represent and generate a circle.
 */
class Circle : public Mesh {
public:
	Circle();

	Circle(int num_of_points);

protected:
	/* Generates the Circle. */
	std::vector<glm::vec3>* generateCircle(int num_of_points);
};

/*
 * getCircle:
 * - Construct a Circle singleton if not constructed.
 */
static Circle* getCircle() {
	static Circle* circle; 
	if (!circle) {
		circle = new Circle{};
	}
	return circle;
}

} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_MODEL_CIRCLE_H_
