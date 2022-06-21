/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Mesh base class.                                    *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/model/mesh.h"

/* External Includes */
#include "glad/gl.h"
#include "glfw/glfw3.h"

/* STL Includes */
#include <algorithm>

namespace wanderers {
namespace simulation {
namespace object {
namespace model {

/*
 * Mesh generateNormals:
 * - Create normals as the same size as the vertices.
 * - Loop through all vertices in set of three (each triangle):
 *   - Calculate the normal as the cross product of the two vectors defining the triangle.
 *   - Set the following three normals as the same as the one calculated.
 */
std::vector<glm::vec3>* Mesh::generateNormals(std::vector<glm::vec3>* vertices, unsigned int mesh_type) {
	std::vector<glm::vec3>* normals = new std::vector<glm::vec3>(vertices->size());
	if (mesh_type == GL_TRIANGLES) {
		for (int i = 0; i < vertices->size(); i += 3) {
			glm::vec3 v1{ vertices->at(i) };
			glm::vec3 v2{ vertices->at(i + 1) };
			glm::vec3 v3{ vertices->at(i + 2) };

			glm::vec3 normal{ glm::normalize(-glm::cross(v2 - v1, v2 - v3)) };

			normals->at(i) = normal;
			normals->at(i + 1) = normal;
			normals->at(i + 2) = normal;
		}
	} else {
		for (int i = 0; i < vertices->size(); i++)
			normals->at(i) = glm::vec3{ 0.0f, 0.0f, 0.0f };
	}
	return normals;
}

/* 
 * Mesh smoothNormals:
 * - For each vertice:
 *   - Add all normals with same vertice position.
 *   - Normalize the new normal.
 */
std::vector<glm::vec3>* Mesh::smoothNormals(std::vector<glm::vec3>* vertices, std::vector<glm::vec3>* normals) {
	std::vector<glm::vec3>* smooth_normals = new std::vector<glm::vec3>(normals->size());
	for (int i = 0; i < normals->size(); i++) {
		auto it{ vertices->begin() };
		while ((it = std::find_if(it, vertices->end(), [&v1 = vertices->at(i)](glm::vec3 v2) { 
			return abs(v1.x - v2.x) < 0.00001
			    && abs(v1.y - v2.y) < 0.00001
				&& abs(v1.z - v2.z) < 0.00001;
			})) != vertices->end()) {
			smooth_normals->at(i) += normals->at(it - vertices->begin());
			it++;
		}
		smooth_normals->at(i) = glm::normalize(smooth_normals->at(i));
	}
	return smooth_normals;
}

std::vector<glm::vec3>* Mesh::getVertices() { return vertices_; }

std::vector<glm::vec3>* Mesh::getNormals() { return normals_; }

float* Mesh::verticesData() {
	return reinterpret_cast<float*>(&vertices_->data()[0]);
}

float* Mesh::normalsData() {
	return reinterpret_cast<float*>(&normals_->data()[0]);
}

int Mesh::size() {
	return sizeof(glm::vec3) * vertices_->size();
}

/*
 * Mesh generateBuffers:
 * - Generate VAO and bind it.
 * - Generate VBO for vertices and bind it.
 * - Generate VBO for normals and bind it.
 * - unbind VAO.
 */
void Mesh::generateBuffers() {
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

void Mesh::bind() { glBindVertexArray(VAO_); }

void Mesh::unbind() { glBindVertexArray(0);  }

/*
 * Mesh Constructor:
 * - Generate vertices.
 * - Generate normals.
 * - Generate Buffers.
 */
Mesh::Mesh(std::vector<glm::vec3>* vertices, unsigned int mesh_type) 
	: vertices_{ vertices }, normals_{ mesh_type != GL_TRIANGLES ? vertices : smoothNormals(vertices_, generateNormals(vertices_, mesh_type)) },
	  VAO_{ 0 }, vertex_VBO_{ 0 }, normal_VBO_{ 0 } {
	generateBuffers();
}

Mesh::~Mesh() {
	delete vertices_;
	delete normals_;
}

} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers
