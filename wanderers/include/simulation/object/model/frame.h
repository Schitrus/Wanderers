
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class holds the frame used in frame rendering.                       *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_MODEL_FRAME_H_
#define WANDERERS_SIMULATION_OBJECT_MODEL_FRAME_H_

/* External Includes */
#include "glm/glm.hpp"

/* STL Includes */
#include <vector>

namespace wanderers {
namespace simulation {
namespace object {
namespace model {

// TODO: Make inherit mesh (when mesh is templated)

/*
 * Class for representing the frame used in frame rendering.
 */
class Frame {
public:
	Frame();

	/* Returns vertices as vectors. */
	std::vector<glm::vec2>* getVertices();

	/* Returns vertices as individual float values. */
	glm::vec2* verticesData();

	glm::vec2* vecData(std::vector<glm::vec2>* vec_data);

	/* Returns the byte size of the vertices. NOTE: normals have same size as vertices for the moment. */
	int size();

	/* Binds the buffers for rendering. */
	void bind();
	/* Unbinds the buffers. */
	void unbind();

	~Frame();
private:
	/* Vertex array object, storing the various buffers. */
	unsigned int VAO_;
	/* Vertex buffer object, storing vertices. */
	unsigned int vertex_VBO_;
	unsigned int coord_VBO_;

	std::vector<glm::vec2>* vertices_;
	std::vector<glm::vec2>* coords_;

	void generateBuffers();
};

} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_MODEL_FRAME_H_
