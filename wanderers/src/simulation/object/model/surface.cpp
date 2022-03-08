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

Surface::Surface() : Surface{ 0, 0.0f } {}

Surface::Surface(int sub_division_level, float roughness) : Icosahedron{ generateSurface(subDivide(generateIcosahedron(), sub_division_level), roughness) } { }

/*
 * Surface generateSurface:
 */
std::vector<glm::vec3>* Surface::generateSurface(std::vector<glm::vec3>* vertices, float roughness) {
	glm::vec3 seed_vec{ static_cast<float>(glfwGetTime()) * 10000.0f };
	for (int i = 0; i < vertices->size(); i++) {
		glm::vec3 vec{ vertices->at(i) };
		
		float displacement = 0.5f * glm::perlin(0.2f * vec + seed_vec) + 0.3f * glm::perlin(vec + seed_vec) + 0.15f * glm::perlin(2.0f * vec + seed_vec) + 0.05f * glm::perlin(4.0f * vec + seed_vec);
		displacement = (displacement + 1.0f) / 2.0f;
		vertices->at(i) = vec * (-displacement * roughness + 1.0f);
	}
		
	return vertices;
}

} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers
