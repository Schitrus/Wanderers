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
#include "simulation/generator/solar_system_generator.h"

/* STL Includes */
#include <typeinfo>
#include <random>
#include <chrono>

#include <iostream>

namespace wanderers {
namespace simulation {

/* Random generator with seed set as the time of execution. */
static std::default_random_engine randomizer(std::chrono::system_clock::now().time_since_epoch().count());

void constructCatalog(std::vector<object::AbstractObject*>& catalog, object::OrbitalSystem* system) {
	for (std::pair<object::AbstractObject*, object::Orbit*> orbit : system->getOrbits()) {
		const std::type_info& type{ typeid(*orbit.first) };
		if (type == typeid(object::Solar) || type == typeid(object::Planet)) {
			catalog.push_back(orbit.first);
		} else if (type == typeid(object::OrbitalSystem)) {
			constructCatalog(catalog, dynamic_cast<object::OrbitalSystem*>(orbit.first));
		}
	}
}

SpaceSimulation::SpaceSimulation(object::CameraObject* camera_object) : solar_systems_{},
                                     group_of_stars_{},
	                                 camera_object_{camera_object},
	                                 astrological_catalog_{},
	                                 is_paused_{false},
	                                 simulation_speed_{1.0f},
	camera_focus_id_{ 0 } {
	addSolarSystem(simulation::generator::generateSolarSystem(40.0f));
	std::uniform_real_distribution<float> temperature(4000.0f, 10000.0f);
	std::uniform_real_distribution<float> size(1.0f, 2.0f);
	for (int i = 0; i < 20; i++)
		addStars(new simulation::object::Stars{ 100, temperature(randomizer), size(randomizer) });

	constructCatalog(astrological_catalog_, solar_systems_.at(0));

	camera_object_->withMutext([this]() { camera_object_->setCameraFocus(astrological_catalog_.at(0)); });
}


void SpaceSimulation::addSolarSystem(object::OrbitalSystem* solar_system) {
	solar_systems_.push_back(solar_system); 
}

void SpaceSimulation::addStars(object::Stars* stars) { 
	group_of_stars_.push_back(stars); 
}

std::vector<object::OrbitalSystem*> SpaceSimulation::getSolarSystems() {
	return solar_systems_; 
}

std::vector<object::Stars*> SpaceSimulation::getGroupOfStars() {
	return group_of_stars_; 
}

unsigned int SpaceSimulation::getSize(object::AbstractObject* parent) {
	unsigned int size{ 1 };

	const std::type_info& parent_type{ typeid(*parent) };
	if (parent_type == typeid(object::OrbitalSystem)) {
		for (std::pair<object::AbstractObject*, object::Orbit*> child : dynamic_cast<simulation::object::OrbitalSystem*>(parent)->getOrbits()) {
			size += getSize(child.first);
		}
	}
	return size;
}

object::AbstractObject* SpaceSimulation::getChildObject(object::AbstractObject* parent, unsigned int child_id) {
	object::AbstractObject* object{parent};
	const std::type_info& parent_type{ typeid(*parent) };
	if (parent_type == typeid(object::OrbitalSystem)) {
		if (child_id == 0) {
			object = dynamic_cast<simulation::object::OrbitalSystem*>(parent)->getOrbits().at(0).first;
		} else {
			for (std::pair<object::AbstractObject*, object::Orbit*> child : dynamic_cast<simulation::object::OrbitalSystem*>(parent)->getOrbits()) {
				unsigned int child_size{ getSize(child.first) };

				if (child_size >= child_id) {
					object = getChildObject(child.first, child_id - 1);
					break;
				} else {
					child_id -= child_size;
				}
			}
		}
	}

	return object;
}

glm::mat4 SpaceSimulation::getOrbitMatrix(object::AbstractObject* parent, unsigned int child_id) {
	glm::mat4 matrix{1.0f};
	const std::type_info& parent_type{ typeid(*parent) };
	if (parent_type == typeid(object::OrbitalSystem)) {
		if (child_id == 0) {
			matrix = glm::mat4{ 1.0f };
		} else {
			for (std::pair<object::AbstractObject*, object::Orbit*> child : dynamic_cast<simulation::object::OrbitalSystem*>(parent)->getOrbits()) {
				unsigned int child_size{ getSize(child.first) };

				if (child_size >= child_id) {
					matrix *= child.first->getMatrix() * getOrbitMatrix(child.first, child_id - 1);
					break;
				} else {
					child_id -= child_size;
				}
			}
		}
	}

	return matrix;
}

glm::mat4 SpaceSimulation::getRotationalMatrix(object::AstronomicalObject* parent, unsigned int child_id) {
	return getChildObject(parent, child_id)->getMatrix();
}

/*
 * SpaceSimulation elapseTime:
 * - If simulation is not paused:
 *   - Elapse time for the orbitee.
 *   - Elapse time for all orbits.
 */
void SpaceSimulation::elapseTime(double seconds) {
	for (object::OrbitalSystem* solar_system : solar_systems_) {
		solar_system->elapseTime(static_cast<int>(!is_paused_) * seconds * simulation_speed_);
	}
	camera_object_->withMutext([this, seconds] { camera_object_->elapseTime(static_cast<int>(!is_paused_) * seconds * simulation_speed_); });
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

unsigned int SpaceSimulation::getCameraFocusId() {
	return camera_focus_id_;
}

void SpaceSimulation::setCameraFocusId(unsigned int focus_id) {
	camera_focus_id_ = focus_id % astrological_catalog_.size();
	camera_object_->withMutext([this]() {camera_object_->setCameraFocus(astrological_catalog_.at(camera_focus_id_)); });
}

unsigned int SpaceSimulation::cycleCameraFocusId() {
	camera_focus_id_ = (camera_focus_id_ + 1) % astrological_catalog_.size();
	setCameraFocusId(camera_focus_id_);
	return camera_focus_id_;
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
