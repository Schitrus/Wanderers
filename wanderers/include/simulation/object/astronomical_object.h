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
#include "common/orientation.h"
#include "simulation/object/abstract_object.h"
#include "simulation/object/aggregate_object.h"

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
	AstronomicalObject(AbstractObject abstract_object, AggregateObject* physical_object);

	AstronomicalObject(AbstractObject abstract_object, AggregateObject* physical_object, 
					   float rotational_angle, float angular_velocity,
					   glm::vec3 rotational_axis, glm::vec3 rotational_face);

	void setPhysicalObject(AggregateObject* physical_object);
	AggregateObject* const getPhysicalObject();

	void setRotationalAxis(glm::vec3 rotational_axis);
	glm::vec3 getRotationalAxis() const;

	void setRotationalFace(glm::vec3 rotational_face);
	glm::vec3 getRotationalFace() const;

	void setRotationalSide(glm::vec3 rotational_side);
	glm::vec3 getRotationalSide() const;

	void setRotationalAngle(float rotational_angle);
	float getRotationalAngle();

	void setAngularVelocity(float angular_velocity);
	float getAngularVelocity();

	/* The parent object that this object depends on. */
	void setParent(AstronomicalObject* parent);
	AstronomicalObject* getParent() const;

	virtual glm::mat4 getRotationalMatrix();

	virtual glm::mat4 getPhysicalMatrix();

	virtual glm::mat4 getMatrix();

	virtual void elapseTime(double seconds);

private:
	/* Object that represents the physical representation of the astronomical object. */
	AggregateObject* physical_object_;

	/* Orientation of the rotation of the astronomical object. */
	common::Orientation rotational_orientation_;

	float rotational_angle_;
	float angular_velocity_;

	AstronomicalObject* parent_;
};

static const AstronomicalObject kAbstractAstronomicalObject{ kDefaultObject, new AggregateObject{kDefaultAggregate} };

} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_ASTRONOMICAL_OBJECT_H_
