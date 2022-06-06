/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the class for aggregate objects.                        *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/aggregate_object.h"

/* STL Includes */
#include <algorithm>

namespace wanderers {
namespace simulation {
namespace object {

AggregateObject::AggregateObject(AbstractObject abstract_object)
	: AbstractObject{ abstract_object }, objects_{} { }

void AggregateObject::addObject(Object* object, glm::vec3 position) {
	objects_.push_back(std::make_pair(object, position));
}

void AggregateObject::removeObject(Object* object) {
	//auto found_object = std::find(objects_.begin(), objects_.end(), [object](std::pair<Object*, glm::vec3> current_object) { return object->getObjectId() == current_object.first->getObjectId(); });
	//if (found_object != objects_.end()) {
	//	objects_.erase(found_object);
	//}
}

std::vector<std::pair<Object*, glm::vec3>> AggregateObject::getObjects() {
	return objects_;
}


void AggregateObject::elapseTime(double seconds) {
	std::for_each(objects_.begin(), objects_.end(), [seconds](std::pair<Object*, glm::vec3> current_object) { current_object.first->elapseTime(seconds); });
}

} // namespace object
} // namespace simulation
} // namespace wanderers
