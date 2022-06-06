/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Class for representation of aggregate objects.                            *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_AGGREGATE_OBJECT_H_
#define WANDERERS_SIMULATION_OBJECT_AGGREGATE_OBJECT_H_

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
 * This class is representation for aggregate objects. 
 */
class AggregateObject : public AbstractObject {
public:
	AggregateObject(AbstractObject abstract_object);

	void addObject(Object* object, glm::vec3 position);
	void removeObject(Object* object);

	std::vector<std::pair<Object*, glm::vec3>> getObjects();

	virtual void elapseTime(double seconds);

private:
	std::vector<std::pair<Object*, glm::vec3>> objects_;
};

} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_AGGREGATE_OBJECT_H_
