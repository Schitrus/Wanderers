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
AstronomicalObject::AstronomicalObject(float radius) : AstronomicalObject{ model::getIcosahedron(), radius } {}

AstronomicalObject::AstronomicalObject(model::Mesh* surface, float radius) : surface_{ surface }, radius_{ radius } {}

std::vector<glm::vec3>* AstronomicalObject::getSurface() {
	return surface_->getVertices();
}

void AstronomicalObject::bind() { surface_->bind(); }
void AstronomicalObject::unbind() { surface_->unbind(); }

float AstronomicalObject::getRadius() {
	return radius_;
}

} // namespace object
} // namespace simulation
} // namespace wanderers
