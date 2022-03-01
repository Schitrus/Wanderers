#ifndef WANDERERS_SIMULATION_OBJECT_MODEL_ICOSAHEDRON_H_
#define WANDERERS_SIMULATION_OBJECT_MODEL_ICOSAHEDRON_H_

#include "glm/glm.hpp"

#include <vector>

class Icosahedron {
public:
	Icosahedron(Icosahedron const&) = delete;

	void operator=(Icosahedron const&) = delete;

	std::vector<glm::vec3>& getVertices();

	std::vector<glm::vec3>& getNormals();

	float* verticesData();

	float* normalsData();

	int size();

	void bind();
	void unbind();

private:

	unsigned int VAO_;
	unsigned int vertex_VBO_;
	unsigned int normal_VBO_;

	static std::vector<glm::vec3>& generateIcosahedron();

	static std::vector<glm::vec3>& generateNormals(std::vector<glm::vec3>& vertices);

	void generateBuffers();

	std::vector<glm::vec3>& vertices_;
	std::vector<glm::vec3>& normals_;

	Icosahedron();

	friend Icosahedron& getIcosahedron();
};

static Icosahedron& getIcosahedron() {
	static Icosahedron icosahedron{};
	return icosahedron;
}

#endif // WANDERERS_SIMULATION_OBJECT_MODEL_ICOSAHEDRON_H_
