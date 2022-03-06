/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Icosahedron class.                                  *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/model/icosahedron.h"

/* External Includes */
#include "glad/gl.h"
#include "glfw/glfw3.h"

namespace wanderers {
namespace simulation {
namespace object {
namespace model {



Icosahedron::Icosahedron() : Icosahedron{ 0 } {}

Icosahedron::Icosahedron(int sub_division_level) : Mesh(subDivide(generateIcosahedron(), sub_division_level), GL_TRIANGLES) { }

// TODO: Make algorithm for generating icosahedron instead of having it hardcoded.

/*
 * Icosahedron generateIcosahedron.
 * - Generates the vertices with each vertex hard coded:
 *   - Generate top triangles.
 *   - Generate side triangles.
 *   - Generate bottom triangles
 */
std::vector<glm::vec3>* Icosahedron::generateIcosahedron() {
	std::vector<glm::vec3>* vertices = new std::vector<glm::vec3>{
		// TOP
		{0.0f, 1.0f, 0.0f},
		{ 0.0f, 0.5f, sqrt(3.0f) / 2.0f },
		{ sin(static_cast<float>(glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(glm::radians(72.0f))) * sqrt(3.0f) / 2.0f },

		{ 0.0f, 1.0f, 0.0f },
		{sin(static_cast<float>(glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(2.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(2.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},

		{0.0f, 1.0f, 0.0f},
		{sin(static_cast<float>(2.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(2.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(3.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(3.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},

		{0.0f, 1.0f, 0.0f},
		{sin(static_cast<float>(3.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(3.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(4.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(4.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},

		{0.0f, 1.0f, 0.0f},
		{sin(static_cast<float>(4.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(4.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},
		{0.0f, 0.5f, sqrt(3.0f) / 2.0f},

		// SIDE
		{0.0f, 0.5f, sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(-3.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-3.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(-2.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-2.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},

		{sin(static_cast<float>(-2.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-2.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},
		{0.0f, 0.5f, sqrt(3.0f) / 2.0f},

		{sin(static_cast<float>(glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(-2.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-2.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(-glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},

		{sin(static_cast<float>(-glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(2.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(2.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},

		{sin(static_cast<float>(2.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(2.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(-glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},
		{0.0f, -0.5f, -sqrt(3.0f) / 2.0f},

		{0.0f, -0.5f, -sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(3.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(3.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(2.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(2.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},

		{sin(static_cast<float>(3.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(3.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},
		{0.0f, -0.5f, -sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(-4.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-4.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},

		{sin(static_cast<float>(-4.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-4.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(4.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(4.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(3.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(3.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},

		{sin(static_cast<float>(4.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(4.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(-4.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-4.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(-3.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-3.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},

		{sin(static_cast<float>(-3.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-3.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},
		{0.0f, 0.5f, sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(4.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f, 0.5f, cos(static_cast<float>(4.0f * glm::radians(72.0f))) * sqrt(3.0f) / 2.0f},

		// BOTTOM
		{0.0f, -1.0f, 0.0f},
		{0.0f, -0.5f, -sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(-glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},

		{0.0f, -1.0f, 0.0f},
		{sin(static_cast<float>(-glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(-2.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-2.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},

		{0.0f, -1.0f, 0.0f},
		{sin(static_cast<float>(-2.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-2.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(-3.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-3.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},

		{0.0f, -1.0f, 0.0f},
		{sin(static_cast<float>(-3.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-3.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},
		{sin(static_cast<float>(-4.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-4.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},

		{0.0f, -1.0f, 0.0f},
		{sin(static_cast<float>(-4.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f, -0.5f, cos(static_cast<float>(-4.0f * glm::radians(72.0f))) * -sqrt(3.0f) / 2.0f},
		{0.0f, -0.5f, -sqrt(3.0f) / 2.0f}
	};
	return vertices;
}

std::vector<glm::vec3>* Icosahedron::subDivide(std::vector<glm::vec3>* vertices, int level) {
	std::vector<glm::vec3>* sub_divided_vertices;
	if (level == 0) {
		sub_divided_vertices = vertices;
	} else {
		sub_divided_vertices = new std::vector<glm::vec3>(4 * vertices->size());
		for (int i = 0; i < vertices->size(); i += 3) {
			glm::vec3 v1{ vertices->at(i) };
			glm::vec3 v2{ vertices->at(i + 1) };
			glm::vec3 v3{ vertices->at(i + 2) };

			glm::vec3 edge1{ v2 - v1 };
			glm::vec3 edge2{ v3 - v2 };
			glm::vec3 edge3{ v1 - v3 };

			glm::vec3 middle1{ glm::normalize(v1 + edge1 / 2.0f) };
			glm::vec3 middle2{ glm::normalize(v2 + edge2 / 2.0f) };
			glm::vec3 middle3{ glm::normalize(v3 + edge3 / 2.0f) };

			sub_divided_vertices->at(i * 4) = v1;
			sub_divided_vertices->at(i * 4 + 1) = middle1;
			sub_divided_vertices->at(i * 4 + 2) = middle3;

			sub_divided_vertices->at(i * 4 + 3) = v2;
			sub_divided_vertices->at(i * 4 + 4) = middle2;
			sub_divided_vertices->at(i * 4 + 5) = middle1;

			sub_divided_vertices->at(i * 4 + 6) = v3;
			sub_divided_vertices->at(i * 4 + 7) = middle3;
			sub_divided_vertices->at(i * 4 + 8) = middle2;

			sub_divided_vertices->at(i * 4 + 9) = middle1;
			sub_divided_vertices->at(i * 4 + 10) = middle2;
			sub_divided_vertices->at(i * 4 + 11) = middle3;
		}
		delete vertices;
		sub_divided_vertices = subDivide(sub_divided_vertices, level - 1);
	}
	return sub_divided_vertices;
}

} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers
