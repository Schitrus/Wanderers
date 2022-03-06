/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of base class for representation of astronomical objects.  *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/astronomical_object.h"

/* Internal Includes */
#include "simulation/object/model/icosahedron.h"

namespace wanderers {
namespace simulation {
namespace object {

	

/*
 * AstronomicalObject Constructor:
 * - Set surface to as the Icosahedron.
 */
AstronomicalObject::AstronomicalObject() : AstronomicalObject{ model::getIcosahedron() } {}

AstronomicalObject::AstronomicalObject(model::Mesh* surface) : surface_{surface} {}

std::vector<glm::vec3>* AstronomicalObject::getSurface() {
	return surface_->getVertices();
}

void AstronomicalObject::bind() { surface_->bind(); }
void AstronomicalObject::unbind() { surface_->unbind(); }

} // namespace object
} // namespace simulation
} // namespace wanderers
