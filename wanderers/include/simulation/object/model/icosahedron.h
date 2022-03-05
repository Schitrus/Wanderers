/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class generates an icosahedron and stores it vertices and normals.   *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_MODEL_ICOSAHEDRON_H_
#define WANDERERS_SIMULATION_OBJECT_MODEL_ICOSAHEDRON_H_

/* External Includes */
#include "glm/glm.hpp"

/* STL Includes */
#include <vector>

namespace simulation {
namespace object {
namespace model {

/*
 * Class to generate an icosahedron shape and store it's vertices and normals.
 * It also takes cares of various buffers used when rendering.
 * Class is a singleton as it always generate the same shape, an icoshaderon.
 */
class Icosahedron {
public:
	/* Singleton, can't construct object. */
	Icosahedron(Icosahedron const&) = delete;

	/* Singleton, can't copy object. */
	void operator=(Icosahedron const&) = delete;

	/* Returns vertices as vectors. */
	std::vector<glm::vec3>& getVertices();
	/* Returns normals as vectors. */
	std::vector<glm::vec3>& getNormals();

	/* Returns vertices as individual float values. */
	float* verticesData();
	/* Returns normals as individual float values. */
	float* normalsData();

	/* Returns the byte size of the vertices. NOTE: normals have same size as vertices for the moment. */
	int size();

	/* Binds the buffers for rendering. */
	void bind();
	/* Unbinds the buffers. */
	void unbind();

private:

	/* Vertex array object, storing the various buffers. */
	unsigned int VAO_;
	/* Vertex buffer object, storing vertices. */
	unsigned int vertex_VBO_;
	/* Vertex buffer object, storing normals. */
	unsigned int normal_VBO_;

	/* Generates the Icosahedron. */
	static std::vector<glm::vec3>& generateIcosahedron();

	/* Generates the normals. Not specific to icosahedron. */
	static std::vector<glm::vec3>& generateNormals(std::vector<glm::vec3>& vertices);

	void generateBuffers();

	std::vector<glm::vec3>& vertices_;
	std::vector<glm::vec3>& normals_;

	/* Singleton constructor. */
	Icosahedron();

	friend Icosahedron& getIcosahedron();
};

/* 
 * getIcosahedron:
 * - Construct Icosahedron singleton if not constructed.
 * - Return refernce of the Icosahedron singleton.
 */
static Icosahedron& getIcosahedron() {
	static Icosahedron icosahedron{};
	return icosahedron;
}


} // namespace model
} // namespace object
} // namespace simulation

#endif // WANDERERS_SIMULATION_OBJECT_MODEL_ICOSAHEDRON_H_
