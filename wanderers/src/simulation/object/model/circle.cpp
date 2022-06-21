/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Circle class.                                       *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/model/circle.h"

/* External Includes */
#include "glad/gl.h"
#include "glfw/glfw3.h"

namespace wanderers {
namespace simulation {
namespace object {
namespace model {

Circle::Circle() : Circle{ 1000 } {}

Circle::Circle(int num_of_points) : Mesh(generateCircle(num_of_points), GL_LINES) { }

std::vector<glm::vec3>* Circle::generateCircle(int num_of_points) {
	std::vector<glm::vec3>* vertices = new std::vector<glm::vec3>(num_of_points);
	for (int i = 0; i < num_of_points; i++) {
		float angle = -360.0f * static_cast<float>(i) / static_cast<float>(num_of_points);
		glm::vec3 point{ sin(glm::radians(angle)), 0.0f, cos(glm::radians(angle)) };
		vertices->at(i) = point;
	}
	return vertices;
}

} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers
