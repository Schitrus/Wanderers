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

#include <iostream>

namespace wanderers {
namespace simulation {
namespace object {
namespace model {

Surface::Surface() : Surface{ 0, 0.0f } {}

unsigned int generateSurfaceTexture(glm::vec3 color) {
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

Surface::Surface(const Surface& surface, glm::vec3 color) : Icosahedron{surface} {
	surface_texture_ = generateSurfaceTexture(color);
}

Surface::Surface(int sub_division_level, float roughness) 
	: Icosahedron{ generateSurface(subDivide(generateIcosahedron(), sub_division_level), roughness) },
	surface_texture_{generateSurfaceTexture(glm::vec3(1.0f))} {}

void Surface::bind() {
	Icosahedron::bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, surface_texture_);
}

void Surface::unbind() {
	glBindTexture(GL_TEXTURE_3D, 0);
	Icosahedron::unbind();
}

/*
 * Surface generateSurface:
 * - Get seed
 * - For each vertice:
 *   - Calculate surface displacement from seed and position and apply it.
 */
std::vector<glm::vec3>* Surface::generateSurface(std::vector<glm::vec3>* vertices, float roughness) {
	glm::vec3 seed_vec{ static_cast<float>(glfwGetTime()) * 10000.0f };
	for (int i = 0; i < vertices->size(); i++) {
		glm::vec3 vec{ vertices->at(i) };
		
		float displacement = 0.5f * glm::perlin(0.2f * vec + seed_vec) + 0.3f * glm::perlin(vec + seed_vec) + 0.15f * glm::perlin(2.0f * vec + seed_vec) + 0.05f * glm::perlin(4.0f * vec + seed_vec);
		displacement = (displacement + 0.5f);
		vertices->at(i) = vec * (displacement * roughness + 1.0f);
	}
		
	return vertices;
}

} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers
