/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Class for representation of objects.                                      *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_OBJECT_H_
#define WANDERERS_SIMULATION_OBJECT_OBJECT_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes */
#include "simulation/object/abstract_object.h"
#include "simulation/object/model/mesh.h"
#include "simulation/object/model/icosahedron.h"

namespace wanderers {
namespace simulation {
namespace object {

/*
 * This class is representation for objects. 
 */
class Object : public AbstractObject {
public:
	Object(AbstractObject abstract_object, model::Mesh* model = model::getIcosahedron());

	void setModel(model::Mesh* model);
	model::Mesh* const getModel() const;

	void bind();
	void unbind();

private:
	model::Mesh* model_;
};

} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_OBJECT_H_
