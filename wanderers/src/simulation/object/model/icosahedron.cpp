#include "simulation/object/model/icosahedron.h"

#include "glad/gl.h"

std::vector<glm::vec3>& Icosahedron::generateIcosahedron() {
	static std::vector<glm::vec3> vertices{
		// ROOF
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

		// SIDES
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

		// FLOOR
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

std::vector<glm::vec3>& Icosahedron::generateNormals(std::vector<glm::vec3>& vertices) {
	static std::vector<glm::vec3> normals(vertices.size());
	for (int i = 0; i < vertices.size(); i+=3) {
		glm::vec3 v1{ vertices.at(i) };
		glm::vec3 v2{ vertices.at(i + 1) };
		glm::vec3 v3{ vertices.at(i + 2) };

		glm::vec3 normal{ glm::normalize( -glm::cross(v2 - v1, v2 - v3) ) };

		normals.at(i)     = normal;
		normals.at(i + 1) = normal;
		normals.at(i + 2) = normal;
	}
	return normals;
}

std::vector<glm::vec3>& Icosahedron::getVertices() { return vertices_; }

std::vector<glm::vec3>& Icosahedron::getNormals() { return normals_; }

float* Icosahedron::verticesData() {
	return reinterpret_cast<float*>(&vertices_.data()[0]);
}

float* Icosahedron::normalsData() {
	return reinterpret_cast<float*>(&normals_.data()[0]);
}

int Icosahedron::size() {
	return sizeof(glm::vec3) * vertices_.size();
}

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

Icosahedron::Icosahedron() : vertices_{ generateIcosahedron() }, normals_{ generateNormals(vertices_) } {
	generateBuffers();
}
