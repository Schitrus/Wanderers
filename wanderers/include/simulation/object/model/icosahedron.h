/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class generates an icosahedron and stores it vertices and normals.   *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_MODEL_ICOSAHEDRON_H_
#define WANDERERS_SIMULATION_OBJECT_MODEL_ICOSAHEDRON_H_

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
 * Class to generate an icosahedron shape and store it's vertices and normals.
 * It also takes cares of various buffers used when rendering.
 */
class Icosahedron : public Mesh {
public:
	Icosahedron();

	Icosahedron(int sub_division_level);

protected:

	/* Generates the Icosahedron. */
	std::vector<glm::vec3>* generateIcosahedron();

	/* Increases the number of triangles 4^n times. */
	std::vector<glm::vec3>* subDivide(std::vector<glm::vec3>* vertices, int level);
};

/*
 * getIcosahedron:
 * - Construct Icosahedron singleton if not constructed.
 */
static Icosahedron* getIcosahedron() {
	static Icosahedron* icosahedron = new Icosahedron{ 4 };
	return icosahedron;
}

} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_MODEL_ICOSAHEDRON_H_
