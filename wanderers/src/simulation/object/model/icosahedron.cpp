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

// TODO: Make algorithm for generating icosahedron instead of having it hardcoded.

/*
 * Icosahedron generateIcosahedron.
 * - Generates the vertices with each vertex hard coded:
 *   - Generate top triangles.
 *   - Generate side triangles.
 *   - Generate bottom triangles
 * - Return the vertices.
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

std::vector<glm::vec3>* Icosahedron::subDivide(std::vector<glm::vec3>* vertices, int n) {
	std::vector<glm::vec3>* sub_divided_vertices = new std::vector<glm::vec3>(4 * vertices->size());
	for (int i = 0; i < vertices->size(); i += 3) {
		glm::vec3 v1{ vertices->at(i) };
		glm::vec3 v2{ vertices->at(i + 1) };
		glm::vec3 v3{ vertices->at(i + 2) };

		glm::vec3 edge1{ v2 - v1 };
		glm::vec3 edge2{ v3 - v2 };
		glm::vec3 edge3{ v1 - v3 };

		glm::vec3 middle1{glm::normalize( v1 + edge1 / 2.0f )};
		glm::vec3 middle2{glm::normalize( v2 + edge2 / 2.0f )};
		glm::vec3 middle3{glm::normalize( v3 + edge3 / 2.0f )};

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
	return n == 1 ? sub_divided_vertices : subDivide(sub_divided_vertices, n - 1);
}

/*
 * Icosahedron generateNormals:
 * - Create normals as the same size as the vertices.
 * - Loop through all vertices in set of three (each triangle):
 *   - Calculate the normal as the cross product of the two vectors defining the triangle.
 *   - Set the following three normals as the same as the one calculated.
 * - Return the normals.
 */
std::vector<glm::vec3>* Icosahedron::generateNormals(std::vector<glm::vec3>* vertices) {
	std::vector<glm::vec3>* normals = new std::vector<glm::vec3>(vertices->size());
	for (int i = 0; i < vertices->size(); i+=3) {
		glm::vec3 v1{ vertices->at(i) };
		glm::vec3 v2{ vertices->at(i + 1) };
		glm::vec3 v3{ vertices->at(i + 2) };

		glm::vec3 normal{ glm::normalize( -glm::cross(v2 - v1, v2 - v3) ) };

		normals->at(i)     = normal;
		normals->at(i + 1) = normal;
		normals->at(i + 2) = normal;
	}
	return normals;
}

std::vector<glm::vec3>* Icosahedron::getVertices() { return vertices_; }

std::vector<glm::vec3>* Icosahedron::getNormals() { return normals_; }

float* Icosahedron::verticesData() {
	return reinterpret_cast<float*>(&vertices_->data()[0]);
}

float* Icosahedron::normalsData() {
	return reinterpret_cast<float*>(&normals_->data()[0]);
}

int Icosahedron::size() {
	return sizeof(glm::vec3) * vertices_->size();
}

/*
 * Icosahedron generateBuffers:
 * - Generate VAO and bind it.
 * - Generate VBO for vertices and bind it.
 * - Generate VBO for normals and bind it.
 * - unbind VAO.
 */
void Icosahedron::generateBuffers() {
	glGenVertexArrays(1, &VAO_);
	glBindVertexArray(VAO_);

	glGenBuffers(1, &vertex_VBO_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO_);
	glBufferData(GL_ARRAY_BUFFER, size(), verticesData(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &normal_VBO_);
	glBindBuffer(GL_ARRAY_BUFFER, normal_VBO_);
	glBufferData(GL_ARRAY_BUFFER, size(), normalsData(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Icosahedron::bind() { glBindVertexArray(VAO_); }

void Icosahedron::unbind() { glBindVertexArray(0);  }

/*
 * Icosahedron Constructor:
 * - Generate vertices.
 * - Generate normals.
 * - Generate Buffers.
 */
Icosahedron::Icosahedron() : vertices_{ subDivide(generateIcosahedron(), 5) }, normals_{ generateNormals(vertices_) } {
	generateBuffers();
}

} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers
