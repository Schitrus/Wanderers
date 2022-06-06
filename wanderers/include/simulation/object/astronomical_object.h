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
#include "simulation/object/abstract_object.h"
#include "simulation/object/object.h"

/* STL Includes */
#include <vector>

namespace wanderers {
namespace simulation {
namespace object {

/*
 * This class is the base for representation of astronomical objects in space simulation. 
 */
class AstronomicalObject : public AbstractObject {
public:
	AstronomicalObject(AbstractObject abstract_object, Object* physical_object, glm::vec3 rotational_axis, float rotational_angle, float angular_velocity);

	Object* const getPhysicalObject();

	glm::vec3 getRotationalAxis();
	float getRotationalAngle();
	float getAngularVelocity();

	virtual glm::mat4 getRotationalMatrix();

	virtual glm::mat4 getMatrix();

	virtual void elapseTime(double seconds);

private:
	/* Object that represents the physical representation of the astronomical object. */
	Object* physical_object_;

	glm::vec3 rotational_axis_;

	float rotational_angle_;

	float angular_velocity_;
};

} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_ASTRONOMICAL_OBJECT_H_
