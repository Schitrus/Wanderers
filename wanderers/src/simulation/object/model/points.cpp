/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Points class.                                       *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/model/points.h"

/* External Includes */
#include "glad/gl.h"
#include "glfw/glfw3.h"

namespace wanderers {
namespace simulation {
namespace object {
namespace model {

Points::Points(std::vector<glm::vec3>* points, std::vector<glm::vec3>* colors) 
	: Mesh(points, GL_POINTS) { 
	if (colors != nullptr) {
		bind();

		unsigned int tmp_VBO;

		glGenBuffers(1, &tmp_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, tmp_VBO);
		glBufferData(GL_ARRAY_BUFFER, size(), vecData(colors), GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribDivisor(1, 1);

		unbind();
	}
}


} // namespace model
} // namespace object
} // namespace simulation
} // namespace wanderers
