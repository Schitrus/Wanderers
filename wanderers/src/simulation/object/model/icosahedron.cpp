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
#include "glm/ext.hpp"

namespace wanderers {
namespace simulation {
namespace object {
namespace model {



Icosahedron::Icosahedron() : Icosahedron{ 0 } {}

unsigned int generateTexture(glm::vec3 color) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_3D, texture);

	unsigned int length = 32;

	unsigned char* data = new unsigned char[length * length * length * 4];
	glm::vec3 seed_vec{ static_cast<float>(glfwGetTime()) * 10000.0f };
	for (int z = 0; z < length; z++) {
		for (int y = 0; y < length; y++) {
			for (int x = 0; x < length; x++) {
				int index = 4 * (z * length * length + y * length + x);
				glm::vec3 coord{ x, y, z };
				float perlin = glm::perlin(0.2f * coord + seed_vec)
					+ 0.3f * glm::perlin(coord + seed_vec)
					+ 0.15f * glm::perlin(2.0f * coord + seed_vec)
					+ 0.05f * glm::perlin(4.0f * coord + seed_vec);
				data[index] = 255 * (0.5f * perlin + 0.5f) * color.x;
				data[index + 1] = 255 * (0.5f * perlin + 0.5f) * color.y;
				data[index + 2] = 255 * (0.5f * perlin + 0.5f) * color.z;
				data[index + 3] = 255 * (0.5f * perlin + 0.5f);
			}
		}
	}

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, length, length, length, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}

Icosahedron::Icosahedron(Icosahedron const& Icosahedron, glm::vec3 color) : Icosahedron{ Icosahedron } {
	texture_ = generateTexture(color);
}

Icosahedron::Icosahedron(int sub_division_level) : Icosahedron(subDivide(generateIcosahedron(), sub_division_level)) { }

Icosahedron::Icosahedron(std::vector<glm::vec3>* vertices) : Mesh(vertices, GL_TRIANGLES), texture_{ generateTexture(glm::vec3(1.0f)) } {}

void Icosahedron::bind() {
	Mesh::bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, texture_);
}

void Icosahedron::unbind() {
	glBindTexture(GL_TEXTURE_3D, 0);
	Mesh::unbind();
}

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

/*
 * Icosahedron subDivide.
 * - If sub division level is zero return vertices.
 * - If sub division level is not zero.
 *   - Generate four new triangles within the triangle.
 *   - Set the vertices whitin the triangles to be of same distance to origo.
 *   - Delete old vertices.
 *   - Call subDivide with a level less.
 * - Return vertices.
 */
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
