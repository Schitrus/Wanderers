/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This base class is the basis for geometric shapes.                        *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_MODEL_MESH_H_
#define WANDERERS_SIMULATION_OBJECT_MODEL_MESH_H_

/* External Includes */
#include "glm/glm.hpp"

/* STL Includes */
#include <vector>

namespace wanderers {
namespace simulation {
namespace object {
namespace model {

// TODO: Make mesh templated with vec size (vec3, vec2 etc...)

/*
 * Base class for representation of geometric shapes used in rendering.
 */
class Mesh {
public:
	Mesh(std::vector<glm::vec3>* vertices, unsigned int mesh_type);

	/* Returns vertices as vectors. */
	std::vector<glm::vec3>* getVertices();
	/* Returns normals as vectors. */
	std::vector<glm::vec3>* getNormals();

	/* Returns vertices as individual float values. */
	glm::vec3* verticesData();
	/* Returns normals as individual float values. */
	glm::vec3* normalsData();

	glm::vec3* vecData(std::vector<glm::vec3>* vecData);

	/* Returns the byte size of the vertices. NOTE: normals have same size as vertices for the moment. */
	int size();

	/* Binds the buffers for rendering. */
	virtual void bind();
	/* Unbinds the buffers. */
	virtual void unbind();

	~Mesh();
private:
	/* Generates the normals. */
	std::vector<glm::vec3>* generateNormals(std::vector<glm::vec3>* vertices, unsigned mesh_type);

	/* Smooths the normals. */
	std::vector<glm::vec3>* smoothNormals(std::vector<glm::vec3>* vertices, std::vector<glm::vec3>* normals);

	/* Vertex array object, storing the various buffers. */
	unsigned int VAO_;
	/* Vertex buffer object, storing vertices. */
	unsigned int vertex_VBO_;
	/* Vertex buffer object, storing normals. */
	unsigned int normal_VBO_;

	std::vector<glm::vec3>* vertices_;
	std::vector<glm::vec3>* normals_;

	void generateBuffers(unsigned int mesh_type);
};

} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_MODEL_MESH_H_
