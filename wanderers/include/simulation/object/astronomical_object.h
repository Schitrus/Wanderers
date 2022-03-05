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
#include "simulation/object/model/icosahedron.h"

/* STL Includes */
#include <vector>

namespace simulation {
namespace object {

// TODO: Generalize the model.

/*
 * This class is the base for representation of astronomical objects in space simulation. 
 */
class AstronomicalObject {
public:
	AstronomicalObject();

	/* Gets the set model for the astronomical object. */
	virtual std::vector<glm::vec3>* getSurface();

	/* Advance the simulation. */
	virtual void elapseTime(double seconds) = 0;

	/* Bind/unbind the astronomical object model for rendering. */
	void bind();
	void unbind();

private:
	/* Model of the astronomical object. NOTE: Model is set to Icosahedron for the time being. */
	model::Icosahedron* surface_;
};

} // namespace object
} // namespace simulation

#endif // WANDERERS_SIMULATION_OBJECT_ASTRONOMICAL_OBJECT_H_
