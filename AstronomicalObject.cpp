#include "AstronomicalObject.h"

AstronomicalObject::AstronomicalObject() : surface_{&getIcosahedron()} {}

std::vector<glm::vec3>* AstronomicalObject::getSurface() {
	return &surface_->getVertices();
}

void AstronomicalObject::bind() { surface_->bind(); }
void AstronomicalObject::unbind() { surface_->unbind(); }