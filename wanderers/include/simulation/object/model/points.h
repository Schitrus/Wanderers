/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class generates points.                                                *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_MODEL_POINTS_H_
#define WANDERERS_SIMULATION_OBJECT_MODEL_POINTS_H_

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
class Points : public Mesh {
public:
	Points(std::vector<glm::vec3>* points, std::vector<glm::vec3>* colors = nullptr);

};

} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_MODEL_POINTS_H_
