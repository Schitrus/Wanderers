#ifndef WANDERERS_SIMULATION_OBJECT_ASTRONOMICAL_OBJECT_H_
#define WANDERERS_SIMULATION_OBJECT_ASTRONOMICAL_OBJECT_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes */
#include "simulation/object/model/icosahedron.h"

/* STL Includes */
#include <vector>

class AstronomicalObject {
public:
	AstronomicalObject();

	virtual std::vector<glm::vec3>* getSurface();

	virtual void elapseTime(double seconds) = 0;

	void bind();
	void unbind();

private:
	Icosahedron* surface_;
};

#endif // WANDERERS_SIMULATION_OBJECT_ASTRONOMICAL_OBJECT_H_
