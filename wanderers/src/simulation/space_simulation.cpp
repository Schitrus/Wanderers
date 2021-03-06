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

/* 
 * Constructs a catalog of astronomical objects from the passed orbital system. 
 */
void constructCatalog(std::vector<object::AstronomicalObject*>& catalog, object::OrbitalSystem* system) {
	for (std::pair<object::AstronomicalObject*, object::Orbit*> orbit : system->getOrbits()) {
		const std::type_info& type{ typeid(*orbit.first) };
		if (type == typeid(object::Solar) || type == typeid(object::Planet)) {
			catalog.push_back(orbit.first);
		} else if (type == typeid(object::OrbitalSystem)) {
			constructCatalog(catalog, dynamic_cast<object::OrbitalSystem*>(orbit.first));
		}
	}
}

/*
 * SpaceSimulation: 
 * - Generate solar system.
 * - Generate stars.
 * - Construct catalog.
 * - Set camera focus to first object in catalog.
 */
SpaceSimulation::SpaceSimulation(object::CameraObject* camera_object) : solar_systems_{},
                                     group_of_stars_{},
	                                 camera_object_{camera_object},
	                                 astrological_catalog_{},
	                                 is_paused_{false},
	                                 simulation_speed_{1.0f},
	camera_focus_id_{ 0 } {
	std::uniform_real_distribution<float> temperature(4000.0f, 10000.0f);
	std::uniform_real_distribution<float> size(0.2f, 2.0f);
	std::uniform_real_distribution<float> cluster_count(0.0f, 5.0f);
	std::uniform_real_distribution<float> cluster_radius(0.2f, 2.0f);
	std::uniform_real_distribution<float> distance_multiplier(1.0f, 3.0f);

	addSolarSystem(generator::generateTheSolarSystem());
	for (int i = 0; i < 25; i++) {
		object::OrbitalSystem* system = generator::generateSolarSystem(10.0f);
		system->setPosition(object::Stars::generateRandomDirection() * 5000.0f * distance_multiplier(randomizer));
		system->setOrientation(object::Stars::generateRandomDirection());
		addSolarSystem(system);
	}

	for (int i = 0; i < 10; i++) {
		std::cout << "Group: " << i << std::endl;
		addStars(new object::Stars{ temperature(randomizer), 1.5f * size(randomizer), 1'000, object::Stars::generateStars(1000, 100) });
		addStars(new object::Stars{ temperature(randomizer), size(randomizer), 100'000, object::Stars::generateGalaxyDisc(1000) });
		float radius = cluster_radius(randomizer);
		glm::vec3 cluster_center = object::Stars::generateRandomDirection();
		for (int j = 0; j < 25; j++) {
			float count = cluster_count(randomizer);
			addStars(new object::Stars{ temperature(randomizer), size(randomizer) * radius * 0.5f, 10'000, object::Stars::generateCluster(count, radius, cluster_center) });
		}
	}

	for (object::OrbitalSystem* system : solar_systems_) {
		constructCatalog(astrological_catalog_, system);
	}

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
