/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the class for representation of a planet.               *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/planet.h"

/* External Includes */
#include "glm/ext.hpp"

/* Internal Includes */
#include "simulation/object/model/icosahedron.h"
#include "simulation/object/model/surface.h"

namespace wanderers {
namespace simulation {
namespace object {

Planet::Planet(float radius, glm::vec3 surface_color)
    : Planet{ AstronomicalObject{ AbstractObject{kOrigo, kUp, kFace, glm::vec3{radius}},
                                  new AggregateObject{Object{model::getDefaultSurface()}}}, surface_color } {}

Planet::Planet(AstronomicalObject astronomical_object, glm::vec3 surface_color) 
	: AstronomicalObject{ astronomical_object }, surface_color_{surface_color} {}

void Planet::setColor(glm::vec3 color) {
	surface_color_ = color;
}

glm::vec3 Planet::getColor() { return surface_color_; }

} // namespace object
} // namespace simulation
} // namespace wanderers
