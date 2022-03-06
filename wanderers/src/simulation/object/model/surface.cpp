/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Surface class.                                      *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/model/surface.h"

/* External Includes */
#include "glad/gl.h"
#include "glfw/glfw3.h"

#include "glm/ext.hpp"

/* STL Includes */
#include <random>
#include <chrono>

namespace wanderers {
namespace simulation {
namespace object {
namespace model {

Surface::Surface() : Surface{ 0 } {}

Surface::Surface(int sub_division_level) : Icosahedron(generateSurface(subDivide(generateIcosahedron(), sub_division_level))) { }

/*
 * Surface generateSurface:
 */
std::vector<glm::vec3>* Surface::generateSurface(std::vector<glm::vec3>* vertices) {
	glm::vec3 seed_vec{ static_cast<float>(glfwGetTime()) * 10000.0f };
	for (int i = 0; i < vertices->size(); i++) {
		glm::vec3 vec{ vertices->at(i) };
		
		float perlin = glm::perlin(vec + seed_vec) + 0.5f * glm::perlin(2.0f * vec + seed_vec) + 0.25f * glm::perlin(4.0f * vec + seed_vec);

		vertices->at(i) = vec * (0.15f * perlin + 1.0f);
	}
		
	return vertices;
}

} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers
