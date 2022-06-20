/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the abstract class for objects.                         *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/object.h"

namespace wanderers {
namespace simulation {
namespace object {

Object::Object(model::Mesh* model) : Object{kDefaultObject, model} {}

Object::Object(AbstractObject abstract_object, model::Mesh* model)
	: AbstractObject{ abstract_object }, model_{ model } { assert(model_ != nullptr); }

void Object::setModel(model::Mesh* model) {
	if (model != nullptr) {
		model_ = model;
	}
}

model::Mesh* const Object::getModel() const {
	return model_;
}

void Object::bind() { model_->bind(); }
void Object::unbind() { model_->unbind(); }

} // namespace object
} // namespace simulation
} // namespace wanderers
