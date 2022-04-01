/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Base class for representation of astronomical objects.                    *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_ASTRONOMICAL_OBJECT_H_
#define WANDERERS_SIMULATION_OBJECT_ASTRONOMICAL_OBJECT_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes */
#include "simulation/object/model/mesh.h"

/* STL Includes */
#include <vector>

namespace wanderers {
namespace simulation {
namespace object {

// TODO: Generalize the model.

/*
 * This class is the base for representation of astronomical objects in space simulation. 
 */
class AstronomicalObject {
public:
	AstronomicalObject(float radius);

	AstronomicalObject(model::Mesh* surface, float radius);

	/* Gets the set model for the astronomical object. */
	virtual std::vector<glm::vec3>* getSurface();

	/* Advance the simulation. */
	virtual void elapseTime(double seconds) = 0;

	/* Bind/unbind the astronomical object model for rendering. */
	void bind();
	void unbind();

	float getRadius();

	virtual glm::mat4 getMatrix();

	const unsigned int id;

protected:

	float radius_;

private:
	/* Model of the astronomical object. */
	model::Mesh* surface_;

	static unsigned int id_counter_;
};

} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_ASTRONOMICAL_OBJECT_H_
