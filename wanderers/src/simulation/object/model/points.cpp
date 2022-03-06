/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Points class.                                       *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/model/points.h"

/* External Includes */
#include "glad/gl.h"
#include "glfw/glfw3.h"

namespace wanderers {
namespace simulation {
namespace object {
namespace model {

Points::Points(std::vector<glm::vec3>* points) : Mesh(points, GL_POINTS) { }


} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers
