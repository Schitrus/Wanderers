#ifndef ASTRONOMICAL_OBJECT_H
#define ASTRONOMICAL_OBJECT_H

#include "glm/glm.hpp"

#include "Icosahedron.h"

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

	friend class SpaceRenderer;
};

#endif // ASTRONOMICAL_OBJECT_H
