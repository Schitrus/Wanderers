/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the class that simulates a solar system.                *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/space_simulation.h"

/* External Includes */
#include "glm/ext.hpp"

/* Internal Includes */
#include "simulation/object/solar.h"
#include "simulation/object/planet.h"

/* STL Includes */
#include <typeinfo>

#include <iostream>

namespace wanderers {
namespace simulation {

SpaceSimulation::SpaceSimulation(render::Camera& camera) : solar_systems_{},
                                     group_of_stars_{},
	                                 camera_{camera},
	                                 is_paused_{false},
	                                 simulation_speed_{1.0f} {}


void SpaceSimulation::addSolarSystem(object::OrbitalSystem* solar_system) { solar_systems_.push_back(solar_system); }

void SpaceSimulation::addStars(object::Stars* stars) { group_of_stars_.push_back(stars); }

std::vector<object::OrbitalSystem*> SpaceSimulation::getSolarSystems() { return solar_systems_; }

std::vector<object::Stars*> SpaceSimulation::getGroupOfStars() { return group_of_stars_; }

unsigned int getSize(object::AstronomicalObject* parent) {
	unsigned int size{ 1 };

	const std::type_info& parent_type{ typeid(*parent) };
	if (parent_type == typeid(object::OrbitalSystem)) {
		for (object::Orbit* child : dynamic_cast<simulation::object::OrbitalSystem*>(parent)->getOrbits()) {
			size += getSize(child->getOrbitor());
		}
	}
	return size;
}

object::AstronomicalObject* getChildObject(object::AstronomicalObject* parent, unsigned int child_id) {
	object::AstronomicalObject* object{parent};
	const std::type_info& parent_type{ typeid(*parent) };
	if (parent_type == typeid(object::OrbitalSystem)) {
		if (child_id == 0) {
			object = dynamic_cast<simulation::object::OrbitalSystem*>(parent)->getOrbitee();
		} else {
			for (object::Orbit* child : dynamic_cast<simulation::object::OrbitalSystem*>(parent)->getOrbits()) {
				unsigned int child_size{ getSize(child->getOrbitor()) };

				if (child_size >= child_id) {
					object = getChildObject(child->getOrbitor(), child_id - 1);
					break;
				} else {
					child_id -= child_size;
				}
			}
		}
	}

	return object;
}

glm::mat4 getOrbitMatrix(object::AstronomicalObject* parent, unsigned int child_id) {
	glm::mat4 matrix{1.0f};
	const std::type_info& parent_type{ typeid(*parent) };
	if (parent_type == typeid(object::OrbitalSystem)) {
		if (child_id == 0) {
			matrix = glm::mat4{ 1.0f };
		} else {
			for (object::Orbit* child : dynamic_cast<simulation::object::OrbitalSystem*>(parent)->getOrbits()) {
				unsigned int child_size{ getSize(child->getOrbitor()) };

				if (child_size >= child_id) {
					matrix *= child->getOrbitMatrix() * getOrbitMatrix(child->getOrbitor(), child_id - 1);
					break;
				} else {
					child_id -= child_size;
				}
			}
		}
	}

	return matrix;
}

/*
 * SpaceSimulation elapseTime:
 * - If simulation is not paused:
 *   - Elapse time for the orbitee.
 *   - Elapse time for all orbits.
 */
void SpaceSimulation::elapseTime(double seconds) {
	if (!is_paused_) {
		for(object::OrbitalSystem* solar_system : solar_systems_)
		solar_system->elapseTime(seconds * simulation_speed_);
	}
	if (camera_.getMode() == render::Camera::Mode::Orbital) {
		std::cout << "Focus ID: " << camera_.getFocusId() << std::endl;
		glm::mat4 orbit_matrix{ getOrbitMatrix(solar_systems_.at(0), camera_.getFocusId()) };
		camera_.setPosition(glm::translate(glm::mat4{1.0f}, glm::vec3{ getChildObject(solar_systems_.at(0), camera_.getFocusId())->getRadius(), 0.0f, 0.0f }) * orbit_matrix * glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
	}
}

void SpaceSimulation::pause() {
	is_paused_ = true;
}

void SpaceSimulation::unpause() {
	is_paused_ = false;
}

bool SpaceSimulation::isPaused() {
	return is_paused_;
}

void SpaceSimulation::setSpeed(double simulation_speed) {
	simulation_speed_ = simulation_speed;
}

double SpaceSimulation::getSpeed() {
	return simulation_speed_;
}

/*
 * SpaceSimulation Destructor:
 * - Destroy solar system.
 * - Destroy stars.
 */
SpaceSimulation::~SpaceSimulation() {
	for(object::OrbitalSystem* solar_system : solar_systems_)
		delete solar_system;
	for (object::Stars* stars : group_of_stars_)
		delete stars;
}

} // namespace simulation
} // namespace wanderers
