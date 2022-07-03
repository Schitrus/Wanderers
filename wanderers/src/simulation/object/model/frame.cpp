/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Frame class.                                        *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/model/frame.h"

/* External Includes */
#include "glad/gl.h"
#include "glfw/glfw3.h"

/* STL Includes */
#include <algorithm>

namespace wanderers {
namespace simulation {
namespace object {
namespace model {

std::vector<glm::vec2>* Frame::getVertices() { return vertices_; }

glm::vec2* Frame::verticesData() {
	return vecData(vertices_);
}


glm::vec2* Frame::vecData(std::vector<glm::vec2>* vec_data) {
	return reinterpret_cast<glm::vec2*>(&vec_data->data()[0]);
}

int Frame::size() {
	return sizeof(glm::vec2) * vertices_->size();
}

/*
 * Mesh generateBuffers:
 * - Generate VAO and bind it.
 * - Generate VBO for vertices and bind it.
 * - Generate VBO for normals and bind it.
 * - unbind VAO.
 */
void Frame::generateBuffers() {
	glGenVertexArrays(1, &VAO_);
	glBindVertexArray(VAO_);

	glGenBuffers(1, &vertex_VBO_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO_);
	glBufferData(GL_ARRAY_BUFFER, size(), verticesData(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &coord_VBO_);
	glBindBuffer(GL_ARRAY_BUFFER, coord_VBO_);
	glBufferData(GL_ARRAY_BUFFER, size(), vecData(coords_), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Frame::bind() { glBindVertexArray(VAO_); }

void Frame::unbind() { glBindVertexArray(0);  }

std::vector<glm::vec2>* generateQuad() {
	std::vector<glm::vec2>* quad = new std::vector<glm::vec2>({ {-1.0f, 1.0f}, {-1.0f, -1.0f}, {1.0f, -1.0f}, {-1.0f, 1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f} });
	return quad;
}

std::vector<glm::vec2>* generateCoords() {
	std::vector<glm::vec2>* coords = new std::vector<glm::vec2>({ {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f} });
	return coords;
}

/*
 * Frame Constructor:
 * - Generate vertices.
 * - Generate coords.
 * - Generate Buffers.
 */
Frame::Frame()
	: vertices_{ generateQuad() }, coords_{ generateCoords() },
	  VAO_{ 0 }, vertex_VBO_{ 0 }, coord_VBO_{ 0 } {
	generateBuffers();
}

Frame::~Frame() {
	delete vertices_;
	delete coords_;
}

} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers
