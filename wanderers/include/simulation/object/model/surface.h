/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class generates surface of a spherical object.                       *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_MODEL_SURFACE_H_
#define WANDERERS_SIMULATION_OBJECT_MODEL_SURFACE_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes */
#include "simulation/object/model/icosahedron.h"

/* STL Includes */
#include <vector>

namespace wanderers {
namespace simulation {
namespace object {
namespace model {

/*
 * Class to generate the surface of a spherical object.
 * It also takes cares of various buffers used when rendering.
 */
class Surface : public Icosahedron {
public:
	Surface();

	Surface(int sub_division_level, float roughness);

protected:
	/* Generates the Surface. */
	std::vector<glm::vec3>* generateSurface(std::vector<glm::vec3>* vertices, float roughness);
};

static Surface* getDefaultSurface() {
	static Surface* surface;
	if (!surface) {
		surface = new Surface{ 3, 0.5 };
	}
	return surface;
}

} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_MODEL_SURFACE_H_
