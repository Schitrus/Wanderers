/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of functions for generating a Solarsystem and parts of it. *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/generator/solar_system_generator.h"

/* External Includes */
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"

/* Internal Includes */
#include "simulation/object/model/icosahedron.h"
#include "simulation/object/model/surface.h"

/* STL Includes */
#include <random>
#include <chrono>
#include <typeinfo>
#include <iostream>

namespace wanderers {
namespace simulation {
namespace generator {

/* Random generator with seed set as the time of execution. */
static std::default_random_engine randomizer( std::chrono::system_clock::now().time_since_epoch().count());

float calcRotationalAxisAngle(float distr) {
	return 170.0f * pow(distr, 80.0f) + 10.0f * pow(distr, 2.0f);
}

float calcOrbitalAxisAngle(float distr) {
	return 179.0f * pow(distr, 80.0f) + 1.0f * pow(distr, 2.0f);
}

glm::vec3 generateRotationalAxis() {
	std::uniform_real_distribution<float> axis(0.0f, 1.0f);
	return glm::vec3{ glm::rotate(glm::mat4{1.0f}, glm::radians(calcRotationalAxisAngle(axis(randomizer))), glm::vec3{1.0f, 0.0f, 0.0f})
					* glm::rotate(glm::mat4{1.0f}, glm::radians(calcRotationalAxisAngle(axis(randomizer))), glm::vec3{0.0f, 0.0f, 1.0f})
					* glm::vec4{0.0f, 1.0f, 0.0f, 0.0f} };
}

glm::vec3 generateOrbitalAxis() {
	std::uniform_real_distribution<float> axis(0.0f, 1.0f);
	return glm::vec3{ glm::rotate(glm::mat4{1.0f}, glm::radians(calcOrbitalAxisAngle(axis(randomizer))), glm::vec3{1.0f, 0.0f, 0.0f})
					* glm::rotate(glm::mat4{1.0f}, glm::radians(calcOrbitalAxisAngle(axis(randomizer))), glm::vec3{0.0f, 0.0f, 1.0f})
					* glm::vec4{0.0f, 1.0f, 0.0f, 0.0f} };
}

void printSolarSystem(object::OrbitalSystem* solar_system, std::string prefix) {
	for (std::pair<object::AbstractObject*, object::Orbit*> orbit : solar_system->getOrbits()) {
		 
		const std::type_info& object_type{ typeid(*orbit.first) };
		if (object_type == typeid(simulation::object::Solar))
			std::cout << prefix << "*" << std::endl;
		else if (object_type == typeid(simulation::object::Planet))
			std::cout << prefix << "o" << std::endl;
		else if (object_type == typeid(simulation::object::OrbitalSystem))
			printSolarSystem(dynamic_cast<simulation::object::OrbitalSystem*>(orbit.first), prefix + "\t");
	}
}

/*
 * generateSolarSystem:
 * - Generate solar with a radius between 0.5 and 4.0 .
 * - First planet system set at a distance twice the radius of the solar.
 * - Until at the edge of the solar system:
 *   - Generate planet system with a radius of between 25% and 200% that of the solar radius.
 *   - Add planet system to the solar system at the current distance.
 *   - increment the distance as big as the planet system.
 * - Return solar system.
 */
object::OrbitalSystem* generateSolarSystem(float radius) {
	std::uniform_real_distribution<float> angle(0.0f, 360.0f);

	// Default Abstract Object
	glm::vec3 default_pos{ 0.0f, 0.0f, 0.0f };
	glm::vec3 default_axis{ 0.0f, 1.0f, 0.0f };
	glm::vec3 default_face{ 0.0f, 0.0f,-1.0f };
	float default_radius{ 1.0f };
	object::AbstractObject default_object{default_pos, default_axis, default_face, default_radius};

	// No Orbit
	float no_orbit_radius{ 0.0f };
	glm::vec3 no_orbit_axis{ 0.0f, 1.0f, 0.0f };
	float no_orbit_angle{ 0.0f };
	float no_orbit_velocity{ 0.0f };
	object::Orbit* no_orbit = new object::Orbit{ no_orbit_radius, no_orbit_axis, no_orbit_angle, no_orbit_velocity };

	// Solar system
	object::OrbitalSystem* solar_system = new object::OrbitalSystem{ default_object };

	// Sun physical object
	object::model::Mesh* sun_model{object::model::getIcosahedron()};
	object::Object* sun_object = new object::Object{default_object, sun_model};

	// Sun
	float sun_radius{ 7.0f };
	object::AbstractObject sun_ao{ default_pos, default_axis, default_face, sun_radius };

	glm::vec3 sun_rot_axis{ 0.0f, 1.0f, 0.0f };
	float sun_rot_angle{ angle(randomizer) };
	float sun_rot_angle_velocity{ 360.0f / 28.0f };
	object::AstronomicalObject sun_astro{ sun_ao, sun_object, sun_rot_axis, sun_rot_angle, sun_rot_angle_velocity };
	float sun_temperature{ 5778 };
	object::Solar* sun = new object::Solar{sun_astro, sun_temperature};

	// Sun orbit
	object::Orbit* sun_orbit = new object::Orbit{*no_orbit};

	sun->setParent(sun_orbit);
	sun_orbit->setParent(solar_system);

	solar_system->addOrbit(sun, sun_orbit);

	// MERCURY
	{
		// planet physical object
		object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
		object::Object* planet_object = new object::Object{ default_object, planet_model };

		// planet
		float planet_radius{ 0.25f };
		object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

		glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.034f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ 360.0f / 59.0f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
		glm::vec3 planet_color{ 0.3f, 0.25f, 0.25f };
		object::Planet* planet = new object::Planet{ planet_astro, planet_color };

		// planet orbit
		float planet_orbit_radius{ 14.0f };
		glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(6.35f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_orbit_angle{ angle(randomizer) };
		float planet_orbit_velocity{ 360.0f / 88.0f };
		object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

		planet->setParent(planet_orbit);
		planet_orbit->setParent(solar_system);

		solar_system->addOrbit(planet, planet_orbit);
	}

	// VENUS
	{
		// planet physical object
		object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
		object::Object* planet_object = new object::Object{ default_object, planet_model };

		// planet
		float planet_radius{ 0.6f };
		object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

		glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(2.64f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ 360.0f / -243.0f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
		glm::vec3 planet_color{ 1.0f, 0.9f, 0.5f };
		object::Planet* planet = new object::Planet{ planet_astro, planet_color };

		// planet orbit
		float planet_orbit_radius{ 28.0f };
		glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(2.15f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_orbit_angle{ angle(randomizer) };
		float planet_orbit_velocity{ 360.0f / 224.0f };
		object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

		planet->setParent(planet_orbit);
		planet_orbit->setParent(solar_system);

		solar_system->addOrbit(planet, planet_orbit);
	}

	// EARTH MOON SYSTEM
	{
		// planet system
		object::OrbitalSystem* planet_system = new object::OrbitalSystem{ default_object };

		// planet physical object
		object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
		object::Object* planet_object = new object::Object{ default_object, planet_model };

		// planet
		float planet_radius{ 0.65f };
		object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

		glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(23.5f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ 360.0f / 1.0f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
		glm::vec3 planet_color{ 0.25f, 0.65f, 1.0f };
		object::Planet* planet = new object::Planet{ planet_astro, planet_color };

		// planet orbit
		object::Orbit* planet_orbit = new object::Orbit{ *no_orbit };

		planet->setParent(planet_orbit);
		planet_orbit->setParent(planet_system);

		planet_system->addOrbit(planet, planet_orbit);

		// MOON
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.17f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(5.145f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ 360.0f / 27.0f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 0.65f, 0.65f, 0.65f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 5.0f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(6.687f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ angle(randomizer) };
			float planet_orbit_velocity{ 360.0f / 27.0f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// planet system orbit
		float planet_system_orbit_radius{ 40.0f };
		glm::vec3 planet_system_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(1.58f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_system_orbit_angle{ angle(randomizer) };
		float planet_system_orbit_velocity{ 360.0f / 365.0f };
		object::Orbit* planet_system_orbit = new object::Orbit{ planet_system_orbit_radius, planet_system_orbit_axis, planet_system_orbit_angle, planet_system_orbit_velocity };

		planet_system->setParent(planet_system_orbit);
		planet_system_orbit->setParent(solar_system);

		solar_system->addOrbit(planet_system, planet_system_orbit);
	}

	// MARS MOON SYSTEM
	{
		// planet system
		object::OrbitalSystem* planet_system = new object::OrbitalSystem{ default_object };

		// planet physical object
		object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
		object::Object* planet_object = new object::Object{ default_object, planet_model };

		// planet
		float planet_radius{ 0.34f };
		object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

		glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(25.19f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ 360.0f / 1.03f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
		glm::vec3 planet_color{ 0.9f, 0.2f, 0.1f };
		object::Planet* planet = new object::Planet{ planet_astro, planet_color };

		// planet orbit
		object::Orbit* planet_orbit = new object::Orbit{ *no_orbit };

		planet->setParent(planet_orbit);
		planet_orbit->setParent(planet_system);

		planet_system->addOrbit(planet, planet_orbit);

		// PHOBOS
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.05f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ 360.0f / 0.32f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 0.6f, 0.5f, 0.3f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 1.0f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(1.093f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ angle(randomizer) };
			float planet_orbit_velocity{ 360.0f / 0.32f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// DEIMOS
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.025f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ 360.0f / 1.26f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 0.9f, 0.8f, 0.5f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 2.5f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.93f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ angle(randomizer) };
			float planet_orbit_velocity{ 360.0f / 1.26f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// planet system orbit
		float planet_system_orbit_radius{ 60.0f };
		glm::vec3 planet_system_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(1.63f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_system_orbit_angle{ angle(randomizer) };
		float planet_system_orbit_velocity{ 360.0f / 687.0f };
		object::Orbit* planet_system_orbit = new object::Orbit{ planet_system_orbit_radius, planet_system_orbit_axis, planet_system_orbit_angle, planet_system_orbit_velocity };

		planet_system->setParent(planet_system_orbit);
		planet_system_orbit->setParent(solar_system);

		solar_system->addOrbit(planet_system, planet_system_orbit);
	}

	// JUPITER MOON SYSTEM
	{
		// planet system
		object::OrbitalSystem* planet_system = new object::OrbitalSystem{ default_object };

		// planet physical object
		object::model::Mesh* planet_model{ object::model::getIcosahedron() };
		object::Object* planet_object = new object::Object{ default_object, planet_model };

		// planet
		float planet_radius{ 3.0f };
		object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

		glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(3.13f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ 360.0f / 0.42f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
		glm::vec3 planet_color{ 1.0f, 0.75f, 0.0f };
		object::Planet* planet = new object::Planet{ planet_astro, planet_color };

		// planet orbit
		object::Orbit* planet_orbit = new object::Orbit{ *no_orbit };

		planet->setParent(planet_orbit);
		planet_orbit->setParent(planet_system);

		planet_system->addOrbit(planet, planet_orbit);

		// IO
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.18f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 1.8f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 0.95f, 1.0f, 0.5f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 10.0f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.05f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ angle(randomizer) };
			float planet_orbit_velocity{ 360.0f / 1.8f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// EUROPA
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.15f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 3.6f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 0.5f, 0.25f, 0.1f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 15.0f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.47f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ angle(randomizer) };
			float planet_orbit_velocity{ 360.0f / 3.6f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// GANYMEDE
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.26f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 7.2f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 0.95f, 1.0f, 0.8f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 25.0f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.20f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ angle(randomizer) };
			float planet_orbit_velocity{ 360.0f / 7.2f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// CALLISTO
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.24f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 16.0f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 0.28f, 0.25f, 0.3f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 30.0f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.20f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ angle(randomizer) };
			float planet_orbit_velocity{ 360.0f / 16.0f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// planet system orbit
		float planet_system_orbit_radius{ 120.0f };
		glm::vec3 planet_system_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.32f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_system_orbit_angle{ angle(randomizer) };
		float planet_system_orbit_velocity{ 360.0f / 4333.0f };
		object::Orbit* planet_system_orbit = new object::Orbit{ planet_system_orbit_radius, planet_system_orbit_axis, planet_system_orbit_angle, planet_system_orbit_velocity };

		planet_system->setParent(planet_system_orbit);
		planet_system_orbit->setParent(solar_system);

		solar_system->addOrbit(planet_system, planet_system_orbit);
	}

	// SATURN MOON SYSTEM
	{
		// planet system
		object::OrbitalSystem* planet_system = new object::OrbitalSystem{ default_object };

		// planet physical object
		object::model::Mesh* planet_model{ object::model::getIcosahedron() };
		object::Object* planet_object = new object::Object{ default_object, planet_model };

		// planet
		float planet_radius{ 2.0f };
		object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

		glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(26.73f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ 360.0f / 0.5f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
		glm::vec3 planet_color{ 1.0f, 0.95f, 0.5f };
		object::Planet* planet = new object::Planet{ planet_astro, planet_color };

		// planet orbit
		object::Orbit* planet_orbit = new object::Orbit{ *no_orbit };

		planet->setParent(planet_orbit);
		planet_orbit->setParent(planet_system);

		planet_system->addOrbit(planet, planet_orbit);

		// MIMAS
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.02f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 0.95f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 0.9f, 0.9f, 0.9f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 5.0f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(1.574f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ angle(randomizer) };
			float planet_orbit_velocity{ 360.0f / 0.95f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// Enceladus
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.025f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 1.37f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 0.8f, 0.85f, 0.9f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 6.5f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.009f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ angle(randomizer) };
			float planet_orbit_velocity{ 360.0f / 1.37f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// TETHYS
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.053f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 1.887f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 0.95f, 0.95f, 0.95f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 9.0f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(1.12f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ angle(randomizer) };
			float planet_orbit_velocity{ 360.0f / 1.887f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// DIONE
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.056f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 2.7f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 0.85f, 0.85f, 0.85f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 12.0f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.019f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ angle(randomizer) };
			float planet_orbit_velocity{ 360.0f / 2.7f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// RHEA
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.076f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 4.5f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 0.9f, 0.9f, 0.95f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 15.0f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.345f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ angle(randomizer) };
			float planet_orbit_velocity{ 360.0f / 4.5f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// TITAN
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.26f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 16.0f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 1.0f, 0.9f, 0.0f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 25.0f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.35f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ angle(randomizer) };
			float planet_orbit_velocity{ 360.0f / 16.0f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// IAPETUS
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.073f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 79.0f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 0.3f, 0.3f, 0.15f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 40.0f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(8.13f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ angle(randomizer) };
			float planet_orbit_velocity{ 360.0f / 79.0f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// planet system orbit
		float planet_system_orbit_radius{ 200.0f };
		glm::vec3 planet_system_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.93f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_system_orbit_angle{ angle(randomizer) };
		float planet_system_orbit_velocity{ 360.0f / 24491.0f };
		object::Orbit* planet_system_orbit = new object::Orbit{ planet_system_orbit_radius, planet_system_orbit_axis, planet_system_orbit_angle, planet_system_orbit_velocity };

		planet_system->setParent(planet_system_orbit);
		planet_system_orbit->setParent(solar_system);

		solar_system->addOrbit(planet_system, planet_system_orbit);
	}

	// URANUS MOON SYSTEM
	{
	// planet system
	object::OrbitalSystem* planet_system = new object::OrbitalSystem{ default_object };

	// planet physical object
	object::model::Mesh* planet_model{ object::model::getIcosahedron() };
	object::Object* planet_object = new object::Object{ default_object, planet_model };

	// planet
	float planet_radius{ 1.3f };
	object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

	glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(97.77f), glm::vec3{ 0.0f, 0.0f, 1.0f });
	float planet_rot_angle{ angle(randomizer) };
	float planet_rot_angle_velocity{ 360.0f / 0.75f };
	object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
	glm::vec3 planet_color{ 0.5f, 1.0f, 1.0f };
	object::Planet* planet = new object::Planet{ planet_astro, planet_color };

	// planet orbit
	object::Orbit* planet_orbit = new object::Orbit{ *no_orbit };

	planet->setParent(planet_orbit);
	planet_orbit->setParent(planet_system);

	planet_system->addOrbit(planet, planet_orbit);

	// MIRANDA
	{
		// planet physical object
		object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
		object::Object* planet_object = new object::Object{ default_object, planet_model };

		// planet
		float planet_radius{ 0.024f };
		object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

		glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ -360.0f / 1.4f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
		glm::vec3 planet_color{ 0.85f, 0.85f, 0.85f };
		object::Planet* planet = new object::Planet{ planet_astro, planet_color };

		// planet orbit
		float planet_orbit_radius{ 4.0f };
		glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(4.23f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_orbit_angle{ angle(randomizer) };
		float planet_orbit_velocity{ 360.0f / 1.4f };
		object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

		planet->setParent(planet_orbit);
		planet_orbit->setParent(planet_system);

		planet_system->addOrbit(planet, planet_orbit);
	}

	// ARIEL
	{
		// planet physical object
		object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
		object::Object* planet_object = new object::Object{ default_object, planet_model };

		// planet
		float planet_radius{ 0.058f };
		object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

		glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ -360.0f / 2.52f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
		glm::vec3 planet_color{ 0.5f, 0.5f, 0.5f };
		object::Planet* planet = new object::Planet{ planet_astro, planet_color };

		// planet orbit
		float planet_orbit_radius{ 6.0f };
		glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.26f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_orbit_angle{ angle(randomizer) };
		float planet_orbit_velocity{ 360.0f / 2.52f };
		object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

		planet->setParent(planet_orbit);
		planet_orbit->setParent(planet_system);

		planet_system->addOrbit(planet, planet_orbit);
	}

	// UMBRIEL
	{
		// planet physical object
		object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
		object::Object* planet_object = new object::Object{ default_object, planet_model };

		// planet
		float planet_radius{ 0.059f };
		object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

		glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ -360.0f / 4.144f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
		glm::vec3 planet_color{ 0.35f, 0.35f, 0.35f };
		object::Planet* planet = new object::Planet{ planet_astro, planet_color };

		// planet orbit
		float planet_orbit_radius{ 9.0f };
		glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.128f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_orbit_angle{ angle(randomizer) };
		float planet_orbit_velocity{ 360.0f / 4.144f };
		object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

		planet->setParent(planet_orbit);
		planet_orbit->setParent(planet_system);

		planet_system->addOrbit(planet, planet_orbit);
	}

	// TITANIA
	{
		// planet physical object
		object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
		object::Object* planet_object = new object::Object{ default_object, planet_model };

		// planet
		float planet_radius{ 0.079f };
		object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

		glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ -360.0f / 8.7f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
		glm::vec3 planet_color{ 0.9f, 0.9f, 0.75f };
		object::Planet* planet = new object::Planet{ planet_astro, planet_color };

		// planet orbit
		float planet_orbit_radius{ 20.0f };
		glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.340f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_orbit_angle{ angle(randomizer) };
		float planet_orbit_velocity{ 360.0f / 8.7f };
		object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

		planet->setParent(planet_orbit);
		planet_orbit->setParent(planet_system);

		planet_system->addOrbit(planet, planet_orbit);
	}

	// OBERON
	{
		// planet physical object
		object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
		object::Object* planet_object = new object::Object{ default_object, planet_model };

		// planet
		float planet_radius{ 0.076f };
		object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

		glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ -360.0f / 13.5f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
		glm::vec3 planet_color{ 0.9f, 0.75f, 0.65f };
		object::Planet* planet = new object::Planet{ planet_astro, planet_color };

		// planet orbit
		float planet_orbit_radius{ 25.0f };
		glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.058f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_orbit_angle{ angle(randomizer) };
		float planet_orbit_velocity{ 360.0f / 13.5f };
		object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

		planet->setParent(planet_orbit);
		planet_orbit->setParent(planet_system);

		planet_system->addOrbit(planet, planet_orbit);
	}

	// planet system orbit
	float planet_system_orbit_radius{ 250.0f };
	glm::vec3 planet_system_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.99f), glm::vec3{ 0.0f, 0.0f, 1.0f });
	float planet_system_orbit_angle{ angle(randomizer) };
	float planet_system_orbit_velocity{ 360.0f / 30689.0f };
	object::Orbit* planet_system_orbit = new object::Orbit{ planet_system_orbit_radius, planet_system_orbit_axis, planet_system_orbit_angle, planet_system_orbit_velocity };

	planet_system->setParent(planet_system_orbit);
	planet_system_orbit->setParent(solar_system);

	solar_system->addOrbit(planet_system, planet_system_orbit);
	}

	// NEPTUNE MOON SYSTEM
	{
	// planet system
	object::OrbitalSystem* planet_system = new object::OrbitalSystem{ default_object };

	// planet physical object
	object::model::Mesh* planet_model{ object::model::getIcosahedron() };
	object::Object* planet_object = new object::Object{ default_object, planet_model };

	// planet
	float planet_radius{ 1.3f };
	object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

	glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(28.32f), glm::vec3{ 0.0f, 0.0f, 1.0f });
	float planet_rot_angle{ angle(randomizer) };
	float planet_rot_angle_velocity{ 360.0f / 0.67f };
	object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
	glm::vec3 planet_color{ 0.65f, 0.75f, 1.0f };
	object::Planet* planet = new object::Planet{ planet_astro, planet_color };

	// planet orbit
	object::Orbit* planet_orbit = new object::Orbit{ *no_orbit };

	planet->setParent(planet_orbit);
	planet_orbit->setParent(planet_system);

	planet_system->addOrbit(planet, planet_orbit);

	// TRITON
	{
		// planet physical object
		object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
		object::Object* planet_object = new object::Object{ default_object, planet_model };

		// planet
		float planet_radius{ 0.14f };
		object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

		glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ -360.0f / 5.88f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
		glm::vec3 planet_color{ 0.75f, 0.7f, 0.65f };
		object::Planet* planet = new object::Planet{ planet_astro, planet_color };

		// planet orbit
		float planet_orbit_radius{ 12.0f };
		glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(129.6f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_orbit_angle{ angle(randomizer) };
		float planet_orbit_velocity{ 360.0f / 5.88f };
		object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

		planet->setParent(planet_orbit);
		planet_orbit->setParent(planet_system);

		planet_system->addOrbit(planet, planet_orbit);
	}

	// planet system orbit
	float planet_system_orbit_radius{ 300.0f };
	glm::vec3 planet_system_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.74f), glm::vec3{ 0.0f, 0.0f, 1.0f });
	float planet_system_orbit_angle{ angle(randomizer) };
	float planet_system_orbit_velocity{ 360.0f / 60195.0f };
	object::Orbit* planet_system_orbit = new object::Orbit{ planet_system_orbit_radius, planet_system_orbit_axis, planet_system_orbit_angle, planet_system_orbit_velocity };

	planet_system->setParent(planet_system_orbit);
	planet_system_orbit->setParent(solar_system);

	solar_system->addOrbit(planet_system, planet_system_orbit);
	}

	// PLUTO MOON SYSTEM
	{
		// planet system
		object::OrbitalSystem* planet_system = new object::OrbitalSystem{ default_object };

		// planet physical object
		object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
		object::Object* planet_object = new object::Object{ default_object, planet_model };

		// planet
		float planet_radius{ 0.12f };
		object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

		glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(122.53f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ 360.0f / 6.4f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
		glm::vec3 planet_color{ 1.0f, 0.95f, 0.75f };
		object::Planet* planet = new object::Planet{ planet_astro, planet_color };

		// planet orbit
		float planet_orbit_radius{ 0.2f };
		glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(129.6f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_orbit_angle{ 0.0f };
		float planet_orbit_velocity{ 360.0f / 6.4f };
		object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

		planet->setParent(planet_orbit);
		planet_orbit->setParent(planet_system);

		planet_system->addOrbit(planet, planet_orbit);

		// CHARON
		{
			// planet physical object
			object::model::Mesh* planet_model{ object::model::getDefaultSurface() };
			object::Object* planet_object = new object::Object{ default_object, planet_model };

			// planet
			float planet_radius{ 0.06f };
			object::AbstractObject planet_ao{ default_pos, default_axis, default_face, planet_radius };

			glm::vec3 planet_rot_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 6.4f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_axis, planet_rot_angle, planet_rot_angle_velocity };
			glm::vec3 planet_color{ 0.75f, 0.7f, 0.65f };
			object::Planet* planet = new object::Planet{ planet_astro, planet_color };

			// planet orbit
			float planet_orbit_radius{ 1.0f };
			glm::vec3 planet_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(119.6f), glm::vec3{ 0.0f, 0.0f, 1.0f });
			float planet_orbit_angle{ 180.0f };
			float planet_orbit_velocity{ 360.0f / 6.4f };
			object::Orbit* planet_orbit = new object::Orbit{ planet_orbit_radius, planet_orbit_axis, planet_orbit_angle, planet_orbit_velocity };

			planet->setParent(planet_orbit);
			planet_orbit->setParent(planet_system);

			planet_system->addOrbit(planet, planet_orbit);
		}

		// planet system orbit
		float planet_system_orbit_radius{ 400.0f };
		glm::vec3 planet_system_orbit_axis = glm::rotate(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::radians(17.16f), glm::vec3{ 0.0f, 0.0f, 1.0f });
		float planet_system_orbit_angle{ angle(randomizer) };
		float planet_system_orbit_velocity{ 360.0f / 90560.0f };
		object::Orbit* planet_system_orbit = new object::Orbit{ planet_system_orbit_radius, planet_system_orbit_axis, planet_system_orbit_angle, planet_system_orbit_velocity };

		planet_system->setParent(planet_system_orbit);
		planet_system_orbit->setParent(solar_system);

		solar_system->addOrbit(planet_system, planet_system_orbit);
	}

	return solar_system;

	//std::uniform_real_distribution<float> solar_size(0.5f, 4.0f);
	//std::uniform_real_distribution<float> start_pos(0.0f, 360.0f);
	//float solar_radius{ solar_size(randomizer) };
	//object::OrbitalSystem* solar_system = new object::OrbitalSystem{ object::AbstractObject{glm::vec3{0.0f}, generateOrbitalAxis(), object::AbstractObject::kFace, 1.0f} };
	//solar_system->addOrbit(generateSolar(solar_radius), new object::Orbit{ 0.0f, object::AbstractObject::kUp, 0.0f, 0.0f });
	//float distance = 2.0f * solar_radius;
	//while (radius >= distance) {
	//	std::uniform_real_distribution<float> planet_system_size(0.25f*solar_radius, 2.0f*solar_radius);
	//	float planet_system_radius{ std::min(planet_system_size(randomizer), radius/2.0f) };
	//	distance += planet_system_radius;
	//	object::OrbitalSystem* planet_system{ generatePlanetSystem(planet_system_radius, distance) };
	//	solar_system->addOrbit(planet_system, new object::Orbit{ distance, generateOrbitalAxis(), 18.0f / sqrt(distance), start_pos(randomizer) });
	//
	//	distance += planet_system_radius;
	//}
	//return solar_system;
}

/*
 * generateSolar:
 * - Randomize the temperature between 1,000K and 10,000K.
 * - Set the given radius.
 * - Randomize the spin of the solar.
 * - generateAxis.
 */
object::Solar* generateSolar(float radius) {
	std::uniform_real_distribution<float> temperature(1000.0f, 10000.0f);
	std::uniform_real_distribution<float> spin(-360.0f, 360.0f);
	return new object::Solar{ object::AstronomicalObject{object::AbstractObject{glm::vec3{0.0f}, object::AbstractObject::kUp, object::AbstractObject::kFace, radius},
														  new object::Object{object::AbstractObject{glm::vec3{0.0f}, object::AbstractObject::kUp, object::AbstractObject::kFace, 1.0f}, new object::model::Surface{4, 0.03f}},
														  generateRotationalAxis(), spin(randomizer), spin(randomizer) },
							   temperature(randomizer) };
}

/*
 * generatePlanetSystem:
 * - Generate planet with a radius between 10% and 35% that of the planet system radius.
 * - Create planet system with the planet as orbitee.
 * - Randomize number of moons:
 *   - Set moon radius as a fraction of remaining size of the planet system.
 *   - Generate moon.
 *   - Make orbit and add it to the planet system.
 *   - Increase distance from moon.
 * - Set the given orbit radius.
 * - Set the orbit speed as dependent to the sqrt of the orbit radius.
 * - Rotation set around the y axis.
 * - Start position around the orbit is randomized.
 */
object::OrbitalSystem* generatePlanetSystem(float radius, float orbit_radius) {

	std::uniform_real_distribution<float> planet_size(0.1f, 0.35f);
	std::uniform_real_distribution<float> start_pos(0.0f, 360.0f);
	
	float planet_radius{ radius * planet_size(randomizer) };
	object::OrbitalSystem* planet_system = new object::OrbitalSystem{ object::AbstractObject{glm::vec3{0.0f}, generateOrbitalAxis(), object::AbstractObject::kSide, 1.0f} };
	planet_system->addOrbit(generatePlanet(planet_radius), new object::Orbit{ 0.0f, object::AbstractObject::kUp, 0.0f, 0.0f});

	float moon_distance{ planet_radius * 2.0f };
	std::uniform_int_distribution<int> moon_distr(0, static_cast<int>((radius - moon_distance) / 0.4f));
	int num_of_moons{ std::min(moon_distr(randomizer), 6) };
	
	for (int i = 0; i < num_of_moons; i++) {
		std::uniform_real_distribution<float> moon_size(0.8f/(num_of_moons - i), 1.2f / (num_of_moons - i));
		float moon_radius{ std::min(0.3f * (radius - moon_distance) * moon_size(randomizer), 0.7f * planet_radius)};
		object::Planet* moon{ generatePlanet(moon_radius) };
		planet_system->addOrbit(moon, new object::Orbit{ moon_distance, generateOrbitalAxis(), 180.0f / sqrt(moon_distance), start_pos(randomizer) });
		moon_distance += 2 * moon_radius + (radius - moon_distance) * moon_size(randomizer);
	}

	return planet_system;
}

/*
 * generatePlanet:
 * - Randomize surface color.
 * - Set the given radius.
 * - Randomize the spin of the planet.
 * - Generate axis.
 */
object::Planet* generatePlanet(float radius) {
	std::uniform_real_distribution<float> spin(-360.0f, 360.0f);
	std::uniform_real_distribution<float> color(0.0f, 1.0f);
	std::uniform_real_distribution<float> axis(0.0f, 1.0f);
	return new object::Planet{ object::AstronomicalObject{object::AbstractObject{glm::vec3{0.0f}, object::AbstractObject::kUp, object::AbstractObject::kFace, radius}, 
														  new object::Object{object::AbstractObject{glm::vec3{0.0f}, object::AbstractObject::kUp, object::AbstractObject::kFace, 1.0f}, 
		                                                  new object::model::Surface{3, 1.0f / log2(radius + 2.0f)}},
		                                                  generateRotationalAxis(), spin(randomizer), spin(randomizer) }, 
		                       glm::vec3{color(randomizer), color(randomizer), color(randomizer)} };
}

} // namespace generator
} // namespace simulation
} // namespace wanderers
