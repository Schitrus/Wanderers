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
#include "simulation/object/aggregate_object.h"
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

/*
 * generateSolarSystem:
 * -
 */
object::OrbitalSystem* generateSolarSystem(float radius) {
	using rando = std::uniform_real_distribution<float>;

	// Solar system
	object::OrbitalSystem* solar_system = new object::OrbitalSystem{};

	float solar_radius{ rando(1.0f, 10.0f)(randomizer) };
	solar_system->addOrbit(generateSolar(solar_radius));
	
	float distance = 2.0f * solar_radius;
	for (int i = 0; i < 10; i++) {
		float planet_radius{ rando(0.1f, 0.5f * solar_radius)(randomizer) };
		float lower_orbit{ planet_radius + distance + rando(0.0f, 0.5f * distance)(randomizer) };
		float upper_orbit{ lower_orbit + rando(0.0f, 0.5f * distance)(randomizer) };
		solar_system->addOrbit(generatePlanet(planet_radius), generateOrbit(lower_orbit, upper_orbit, solar_radius));
		distance = upper_orbit + planet_radius;
	}

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
	return solar_system;
}

/*
 * generateSolar:
 * - Generate the physical object.
 * - Generate the object parameters: Radius.
 * - Generate the astronomical object: Rotational parameters.
 * - Generate the Solar with a temperature between 1,000 and 10,000.
 */
object::Solar* generateSolar(float radius) {
	std::uniform_real_distribution<float> temperature(1000.0f, 10000.0f);
	std::uniform_real_distribution<float> angle(-360.0f, 360.0f);
	std::uniform_real_distribution<float> angular_velocity(-360.0f, 360.0f);

	// Solar physical object
	object::AggregateObject* physical_object{ new object::AggregateObject{ new object::Object{object::model::getIcosahedron() } } };

	// Solar
	object::AbstractObject abstract_object{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
								            object::AbstractObject::kFace, glm::vec3{radius} };

	object::AstronomicalObject astronomical_object{ abstract_object, physical_object, angle(randomizer), angular_velocity(randomizer),
										    generateRotationalAxis(), object::AbstractObject::kFace };
	object::Solar* solar = new object::Solar{ astronomical_object, temperature(randomizer) };

	return solar;
}

/*
 * generatePlanetSystem:
 * - 
 */
object::OrbitalSystem* generatePlanetSystem(float radius, float orbit_radius) {

	//std::uniform_real_distribution<float> planet_size(0.1f, 0.35f);
	//std::uniform_real_distribution<float> start_pos(0.0f, 360.0f);
	//
	//float planet_radius{ radius * planet_size(randomizer) };
	//object::OrbitalSystem* planet_system = new object::OrbitalSystem{ object::AbstractObject{glm::vec3{0.0f}, generateOrbitalAxis(), object::AbstractObject::kSide, 1.0f} };
	//planet_system->addOrbit(generatePlanet(planet_radius), new object::Orbit{ 0.0f, object::AbstractObject::kUp, 0.0f, 0.0f});
	//
	//float moon_distance{ planet_radius * 2.0f };
	//std::uniform_int_distribution<int> moon_distr(0, static_cast<int>((radius - moon_distance) / 0.4f));
	//int num_of_moons{ std::min(moon_distr(randomizer), 6) };
	//
	//for (int i = 0; i < num_of_moons; i++) {
	//	std::uniform_real_distribution<float> moon_size(0.8f/(num_of_moons - i), 1.2f / (num_of_moons - i));
	//	float moon_radius{ std::min(0.3f * (radius - moon_distance) * moon_size(randomizer), 0.7f * planet_radius)};
	//	object::Planet* moon{ generatePlanet(moon_radius) };
	//	planet_system->addOrbit(moon, new object::Orbit{ moon_distance, generateOrbitalAxis(), 180.0f / sqrt(moon_distance), start_pos(randomizer) });
	//	moon_distance += 2 * moon_radius + (radius - moon_distance) * moon_size(randomizer);
	//}
	//
	//return planet_system;

	return new object::OrbitalSystem{};
}

/*
 * generateOrbit:
 * - Generates the parameters defining an orbit:
 *   - Eccentricity.
 *   - Semi-major axis.
 *   - Inclination.
 *   - Longitude of the acending node.
 *   - Argument of periapsis.
 *   - True anomaly.
 *   - Angular velocity
 */
object::Orbit* generateOrbit(float lower_radius, float upper_radius, float velocity_factor) {
	using rando = std::uniform_real_distribution<float>;

	rando angle(0.0f, glm::radians(360.0f));
	rando prob(0.0f, 1.0f);

	float semimajor_axis = rando(lower_radius, upper_radius)(randomizer);
	float max_eccentricity = sqrt(1 - (lower_radius * lower_radius) / (semimajor_axis * semimajor_axis));
	float eccentricity = rando(0.0f, max_eccentricity)(randomizer);
	eccentricity *= eccentricity;
	float longitude_of_acending_node = angle(randomizer);
	float argument_of_periapsis = angle(randomizer);
	float inclination = angle(randomizer);
	for (int i = 0; i < 6; i++) inclination *= prob(randomizer);
	float true_anomaly = angle(randomizer);
	float angular_velocity = 1000.0f * velocity_factor/( semimajor_axis* semimajor_axis);
	object::Orbit* orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
											  argument_of_periapsis, true_anomaly, angular_velocity };

	return orbit;
}

/*
 * generatePlanet:
 * - Generate the physical object.
 * - Generate the object parameters: Radius.
 * - Generate the astronomical object: Rotational parameters.
 * - Generate the planet with random color.
 */
object::Planet* generatePlanet(float radius) {
	std::uniform_real_distribution<float> color(0.0f, 1.0f);
	std::uniform_real_distribution<float> roughness(0.0f, 1.0f);
	std::uniform_real_distribution<float> angle(-360.0f, 360.0f);
	std::uniform_real_distribution<float> angular_velocity(-360.0f, 360.0f);

	// planet physical object
	object::AggregateObject* physical_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

	// planet
	object::AbstractObject abstract_object{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											object::AbstractObject::kFace, glm::vec3{radius} };

	object::AstronomicalObject astronomical_object{ abstract_object, physical_object, angle(randomizer), angular_velocity(randomizer),
											 generateRotationalAxis(), object::AbstractObject::kFace };
	object::Planet* planet = new object::Planet{ astronomical_object, glm::vec3{ color(randomizer), color(randomizer), color(randomizer) } };

	return planet;
}

object::OrbitalSystem* generateTheSolarSystem() {
	std::uniform_real_distribution<float> angle(0.0f, 360.0f);

	// Solar system
	object::OrbitalSystem* solar_system = new object::OrbitalSystem{};

	// Sun physical object
	object::AggregateObject* sun_object{ new object::AggregateObject{ new object::Object{object::model::getIcosahedron() } } };

	// Sun
	float sun_radius{ 7.0f };
	object::AbstractObject sun_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
								   object::AbstractObject::kFace, glm::vec3{sun_radius} };

	float sun_rot_angle{ angle(randomizer) };
	float sun_rot_angle_velocity{ 360.0f / 28.0f };
	object::AstronomicalObject sun_astro{ sun_ao, sun_object, sun_rot_angle, sun_rot_angle_velocity,
										  object::AbstractObject::kUp, object::AbstractObject::kFace };
	float sun_temperature{ 5778 };
	object::Solar* sun = new object::Solar{ sun_astro, sun_temperature };

	solar_system->addOrbit(sun);

	// MERCURY
	{
		// planet physical object
		object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ new object::model::Surface(*object::model::getDefaultSurface(), glm::vec3(0.4f, 0.2f, 0.2f)) } } };

		// planet
		float planet_radius{ 0.25f };
		object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
										  object::AbstractObject::kFace, glm::vec3{planet_radius} };

		glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.034f), object::AbstractObject::kFace);
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ 360.0f / 59.0f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
												 planet_rot_axis, object::AbstractObject::kFace };
		object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.3f, 0.25f, 0.25f } };

		// planet orbit
		constexpr float eccentricity = 0.205630f;
		constexpr float semimajor_axis = 14.0f;
		constexpr float longitude_of_acending_node = glm::radians(48.331f);
		constexpr float argument_of_perihelion = glm::radians(29.124f);
		constexpr float inclination = glm::radians(6.35f);
		const float true_anomaly = angle(randomizer);
		constexpr float angular_velocity = 360.0f / 88.0f;
		object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
														argument_of_perihelion, true_anomaly, angular_velocity };

		solar_system->addOrbit(planet, planet_orbit);
	}

	// VENUS

	{
		// planet physical object
		object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ new object::model::Surface(*object::model::getDefaultSurface(), glm::vec3(0.9f, 0.6f, 0.5f)) } } };


		// planet
		float planet_radius{ 0.6f };
		object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
										  object::AbstractObject::kFace, glm::vec3{planet_radius} };

		glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(2.64f), object::AbstractObject::kFace);
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ 360.0f / -243.0f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
												 planet_rot_axis, object::AbstractObject::kFace };
		object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 1.0f, 0.95f, 0.7f } };

		// planet orbit
		constexpr float eccentricity = 0.006772f;
		constexpr float semimajor_axis = 28.0f;
		constexpr float longitude_of_acending_node = glm::radians(76.680f);
		constexpr float argument_of_perihelion = glm::radians(54.884f);
		constexpr float inclination = glm::radians(2.15f);
		const float true_anomaly = angle(randomizer);
		constexpr float angular_velocity = 360.0f / 224.0f;
		object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
														 argument_of_perihelion, true_anomaly, angular_velocity };

		solar_system->addOrbit(planet, planet_orbit);
	}

	// EARTH MOON SYSTEM
	{
		// planet system
		object::OrbitalSystem* planet_system = new object::OrbitalSystem{};

		// planet physical object
		object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ new object::model::Surface(*object::model::getDefaultSurface(), glm::vec3(0.0f, 0.4f, 0.0f)) } } };

		// planet
		float planet_radius{ 0.65f };
		object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
										  object::AbstractObject::kFace, glm::vec3{planet_radius} };

		glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(23.5f), object::AbstractObject::kFace);
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ 360.0f / 1.0f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
												 planet_rot_axis, object::AbstractObject::kFace };
		object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.10f, 0.35f, 0.5f } };

		planet_system->addOrbit(planet);

		// MOON
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ new object::model::Surface(*object::model::getDefaultSurface(), glm::vec3(0.45f, 0.45f, 0.45f)) } } };

			// planet
			float planet_radius{ 0.17f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(5.145f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ 360.0f / 27.0f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };
			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.65f, 0.65f, 0.65f } };

			// planet orbit
			constexpr float eccentricity = 0.0549f;
			constexpr float semimajor_axis = 5.0f;
			const     float longitude_of_acending_node = angle(randomizer);
			const     float argument_of_perihelion = angle(randomizer);
			constexpr float inclination = glm::radians(5.145f);
			const     float true_anomaly = angle(randomizer);
			constexpr float angular_velocity = 360.0f / 27.0f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// planet system orbit
		constexpr float eccentricity = 0.0167086f;
		constexpr float semimajor_axis = 40.0f;
		constexpr float longitude_of_acending_node = glm::radians(-11.26064f);
		constexpr float argument_of_perihelion = glm::radians(114.20783f);
		constexpr float inclination = glm::radians(1.57869f);
		const float true_anomaly = angle(randomizer);
		constexpr float angular_velocity = 360.0f / 365.0f;
		object::Orbit* planet_system_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
																argument_of_perihelion, true_anomaly, angular_velocity };

		solar_system->addOrbit(planet_system, planet_system_orbit);
	}



	// MARS MOON SYSTEM
	{
		// planet system
		object::OrbitalSystem* planet_system = new object::OrbitalSystem{};

		// planet physical object
		object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

		// planet
		float planet_radius{ 0.34f };
		object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
										  object::AbstractObject::kFace, glm::vec3{planet_radius} };

		glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(25.19f), object::AbstractObject::kFace);
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ 360.0f / 1.03f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
												 planet_rot_axis, object::AbstractObject::kFace };
		object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.9f, 0.2f, 0.1f } };

		planet_system->addOrbit(planet);

		// PHOBOS
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

			// planet
			float planet_radius{ 0.05f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ 360.0f / 0.32f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.6f, 0.5f, 0.3f } };

			// planet orbit
			constexpr float eccentricity = 0.0151f;
			constexpr float semimajor_axis = 1.0f;
			const     float longitude_of_acending_node = angle(randomizer);
			const     float argument_of_perihelion = angle(randomizer);
			constexpr float inclination = glm::radians(1.093f);
			const     float true_anomaly = angle(randomizer);
			constexpr float angular_velocity = 360.0f / 0.32f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// DEIMOS
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

			// planet
			float planet_radius{ 0.025f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ 360.0f / 1.26f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.9f, 0.8f, 0.5f } };

			constexpr float eccentricity = 0.00033f;
			constexpr float semimajor_axis = 2.5f;
			const     float longitude_of_acending_node = angle(randomizer);
			const     float argument_of_perihelion = angle(randomizer);
			constexpr float inclination = glm::radians(0.93f);
			const     float true_anomaly = angle(randomizer);
			constexpr float angular_velocity = 360.0f / 1.26f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// planet system orbit
		constexpr float eccentricity = 0.0934f;
		constexpr float semimajor_axis = 60.0f;
		constexpr float longitude_of_acending_node = glm::radians(49.57854f);
		constexpr float argument_of_perihelion = glm::radians(286.5f);
		constexpr float inclination = glm::radians(1.63f);
		const float true_anomaly = angle(randomizer);
		constexpr float angular_velocity = 360.0f / 687.0f;
		object::Orbit* planet_system_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
																argument_of_perihelion, true_anomaly, angular_velocity };

		solar_system->addOrbit(planet_system, planet_system_orbit);
	}

	// JUPITER MOON SYSTEM
	{
		// planet system
		object::OrbitalSystem* planet_system = new object::OrbitalSystem{ };

		// planet physical object
		object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

		// planet
		float planet_radius{ 3.0f };
		object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
										  object::AbstractObject::kFace, glm::vec3{planet_radius} };

		glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(3.13f), object::AbstractObject::kFace);
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ 360.0f / 0.42f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

		object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 1.0f, 0.75f, 0.0f } };

		planet_system->addOrbit(planet);

		// IO
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

			// planet
			float planet_radius{ 0.18f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 1.8f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.95f, 1.0f, 0.5f } };

			// planet orbit
			constexpr float eccentricity = 0.0041f;
			constexpr float semimajor_axis = 10.0f;
			const     float longitude_of_acending_node = angle(randomizer);
			const     float argument_of_perihelion = angle(randomizer);
			constexpr float inclination = glm::radians(0.05f);
			const     float true_anomaly = angle(randomizer);
			constexpr float angular_velocity = 360.0f / 1.8f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// EUROPA
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

			// planet
			float planet_radius{ 0.15f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 3.6f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.5f, 0.25f, 0.1f } };

			// planet orbit
			constexpr float eccentricity = 0.0009f;
			constexpr float semimajor_axis = 15.0f;
			const     float longitude_of_acending_node = angle(randomizer);
			const     float argument_of_perihelion = angle(randomizer);
			constexpr float inclination = glm::radians(0.47f);
			const     float true_anomaly = angle(randomizer);
			constexpr float angular_velocity = 360.0f / 3.6f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// GANYMEDE
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

			// planet
			float planet_radius{ 0.26f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 7.2f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.95f, 1.0f, 0.8f } };

			// planet orbit
			constexpr float eccentricity = 0.0013f;
			constexpr float semimajor_axis = 25.0f;
			const     float longitude_of_acending_node = angle(randomizer);
			const     float argument_of_perihelion = angle(randomizer);
			constexpr float inclination = glm::radians(0.20f);
			const     float true_anomaly = angle(randomizer);
			constexpr float angular_velocity = 360.0f / 7.2f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// CALLISTO
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

			// planet
			float planet_radius{ 0.26f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 16.0f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
																 planet_rot_axis, object::AbstractObject::kFace };

			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.28f, 0.25f, 0.3f } };

			// planet orbit
			constexpr float eccentricity = 0.0074f;
			constexpr float semimajor_axis = 30.0f;
			const     float longitude_of_acending_node = angle(randomizer);
			const     float argument_of_perihelion = angle(randomizer);
			constexpr float inclination = glm::radians(0.192f);
			const     float true_anomaly = angle(randomizer);
			constexpr float angular_velocity = 360.0f / 16.0f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// planet system orbit
		constexpr float eccentricity               = 0.0489f;
		constexpr float semimajor_axis             = 120.0f;
		constexpr float longitude_of_acending_node = glm::radians(100.464f);
		constexpr float argument_of_perihelion     = glm::radians(273.867f);
		constexpr float inclination                = glm::radians(0.32f);
		const float true_anomaly                   = angle(randomizer);
		constexpr float angular_velocity           = 360.0f / 4333.0f;
		object::Orbit* planet_system_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
																argument_of_perihelion, true_anomaly, angular_velocity };

		solar_system->addOrbit(planet_system, planet_system_orbit);
	}

	// SATURN MOON SYSTEM
	{
		// planet system
		object::OrbitalSystem* planet_system = new object::OrbitalSystem{ };

		// planet physical object
		object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

		// planet
		float planet_radius{ 2.0f };
		object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
										  object::AbstractObject::kFace, glm::vec3{planet_radius} };

		glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(26.73f), object::AbstractObject::kFace);
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ 360.0f / 0.5f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

		object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 1.0f, 0.95f, 0.5f } };

		planet_system->addOrbit(planet);

		// MIMAS
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

			// planet
			float planet_radius{ 0.02f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 0.95f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.9f, 0.9f, 0.9f } };

			// planet orbit
			constexpr float eccentricity = 0.0196f;
			constexpr float semimajor_axis = 5.0f;
			const     float longitude_of_acending_node = angle(randomizer);
			const     float argument_of_perihelion = angle(randomizer);
			constexpr float inclination = glm::radians(1.574f);
			const     float true_anomaly = angle(randomizer);
			constexpr float angular_velocity = 360.0f / 0.95f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// Enceladus
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

			// planet
			float planet_radius{ 0.025f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 1.37f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
											         planet_rot_axis, object::AbstractObject::kFace };

			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.8f, 0.85f, 0.9f } };

			// planet orbit
			constexpr float eccentricity = 0.0047f;
			constexpr float semimajor_axis = 6.5f;
			const     float longitude_of_acending_node = angle(randomizer);
			const     float argument_of_perihelion = angle(randomizer);
			constexpr float inclination = glm::radians(0.009f);
			const     float true_anomaly = angle(randomizer);
			constexpr float angular_velocity = 360.0f / 1.37f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// TETHYS
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

			// planet
			float planet_radius{ 0.053f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 1.887f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.95f, 0.95f, 0.95f } };

			// planet orbit
			constexpr float eccentricity = 0.0001f;
			constexpr float semimajor_axis = 9.0f;
			const     float longitude_of_acending_node = angle(randomizer);
			const     float argument_of_perihelion = angle(randomizer);
			constexpr float inclination = glm::radians(1.12f);
			const     float true_anomaly = angle(randomizer);
			constexpr float angular_velocity = 360.0f / 1.887f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// DIONE
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

			// planet
			float planet_radius{ 0.056f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 2.7f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.85f, 0.85f, 0.85f } };

			// planet orbit
			constexpr float eccentricity = 0.0022f;
			constexpr float semimajor_axis = 12.0f;
			const     float longitude_of_acending_node = angle(randomizer);
			const     float argument_of_perihelion = angle(randomizer);
			constexpr float inclination = glm::radians(0.019f);
			const     float true_anomaly = angle(randomizer);
			constexpr float angular_velocity = 360.0f / 2.7f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// RHEA
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

			// planet
			float planet_radius{ 0.076f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 4.5f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.9f, 0.9f, 0.95f } };

			// planet orbit
			constexpr float eccentricity = 0.0012583f;
			constexpr float semimajor_axis = 15.0f;
			const     float longitude_of_acending_node = angle(randomizer);
			const     float argument_of_perihelion = angle(randomizer);
			constexpr float inclination = glm::radians(0.345f);
			const     float true_anomaly = angle(randomizer);
			constexpr float angular_velocity = 360.0f / 4.5f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// TITAN
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

			// planet
			float planet_radius{ 0.26f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 16.0f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 1.0f, 0.9f, 0.0f } };

			// planet orbit
			constexpr float eccentricity = 0.0288f;
			constexpr float semimajor_axis = 25.0f;
			const     float longitude_of_acending_node = angle(randomizer);
			const     float argument_of_perihelion = angle(randomizer);
			constexpr float inclination = glm::radians(0.35f);
			const     float true_anomaly = angle(randomizer);
			constexpr float angular_velocity = 360.0f / 16.0f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// IAPETUS
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

			// planet
			float planet_radius{ 0.073f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ -360.0f / 79.0f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.3f, 0.3f, 0.15f } };

			// planet orbit
			constexpr float eccentricity = 0.0276812f;
			constexpr float semimajor_axis = 40.0f;
			const     float longitude_of_acending_node = angle(randomizer);
			const     float argument_of_perihelion = angle(randomizer);
			constexpr float inclination = glm::radians(8.13f);
			const     float true_anomaly = angle(randomizer);
			constexpr float angular_velocity = 360.0f / 79.0f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// planet system orbit
		constexpr float eccentricity               = 0.0489f;
		constexpr float semimajor_axis             = 200.0f;
		constexpr float longitude_of_acending_node = glm::radians(113.665f);
		constexpr float argument_of_perihelion     = glm::radians(339.392f);
		constexpr float inclination                = glm::radians(0.93f);
		const float true_anomaly                   = angle(randomizer);
		constexpr float angular_velocity           = 360.0f / 24491.0f;
		object::Orbit* planet_system_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
																argument_of_perihelion, true_anomaly, angular_velocity };

		solar_system->addOrbit(planet_system, planet_system_orbit);
	}

	

	// URANUS MOON SYSTEM
	{
	// planet system
	object::OrbitalSystem* planet_system = new object::OrbitalSystem{ };

	// planet physical object
	object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

	// planet
	float planet_radius{ 1.3f };
	object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
									  object::AbstractObject::kFace, glm::vec3{planet_radius} };

	glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(97.77f), object::AbstractObject::kFace);
	float planet_rot_angle{ angle(randomizer) };
	float planet_rot_angle_velocity{ 360.0f / 0.75f };
	object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
										     planet_rot_axis, object::AbstractObject::kFace };

	object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.5f, 1.0f, 1.0f } };

	planet_system->addOrbit(planet);

	// MIRANDA
	{
		// planet physical object
		object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

		// planet
		float planet_radius{ 0.024f };
		object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
										  object::AbstractObject::kFace, glm::vec3{planet_radius} };

		glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ -360.0f / 1.4f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

		object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.85f, 0.85f, 0.85f } };

		// planet orbit
		constexpr float eccentricity = 0.0013;
		constexpr float semimajor_axis = 4.0f;
		const     float longitude_of_acending_node = 0.0f;
		const     float argument_of_perihelion = angle(randomizer);
		constexpr float inclination = glm::radians(4.23f + 97.77f);
		const     float true_anomaly = angle(randomizer);
		constexpr float angular_velocity = 360.0f / 1.4f;
		object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
														 argument_of_perihelion, true_anomaly, angular_velocity };
		
		planet_system->addOrbit(planet, planet_orbit);
	}

	// ARIEL
	{
		// planet physical object
		object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

		// planet
		float planet_radius{ 0.058f };
		object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
										  object::AbstractObject::kFace, glm::vec3{planet_radius} };

		glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ -360.0f / 2.52f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

		object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.5f, 0.5f, 0.5f } };

		// planet orbit
		constexpr float eccentricity = 0.0012f;
		constexpr float semimajor_axis = 6.0f;
		const     float longitude_of_acending_node = 0.0f;
		const     float argument_of_perihelion = angle(randomizer);
		constexpr float inclination = glm::radians(0.26f + 97.77f);
		const     float true_anomaly = angle(randomizer);
		constexpr float angular_velocity = 360.0f / 2.52f;
		object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
														 argument_of_perihelion, true_anomaly, angular_velocity };

		planet_system->addOrbit(planet, planet_orbit);
	}

	// UMBRIEL
	{
		// planet physical object
		object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

		// planet
		float planet_radius{ 0.059f };
		object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
										  object::AbstractObject::kFace, glm::vec3{planet_radius} };

		glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ -360.0f / 4.144f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

		object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.35f, 0.35f, 0.35f } };

		// planet orbit
		constexpr float eccentricity = 0.0039f;
		constexpr float semimajor_axis = 9.0f;
		const     float longitude_of_acending_node = 0.0f;
		const     float argument_of_perihelion = angle(randomizer);
		constexpr float inclination = glm::radians(0.128f + 97.77f);
		const     float true_anomaly = angle(randomizer);
		constexpr float angular_velocity = 360.0f / 4.144f;
		object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
														 argument_of_perihelion, true_anomaly, angular_velocity };

		planet_system->addOrbit(planet, planet_orbit);
	}

	// TITANIA
	{
		// planet physical object
		object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

		// planet
		float planet_radius{ 0.079f };
		object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
										  object::AbstractObject::kFace, glm::vec3{planet_radius} };

		glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ -360.0f / 8.7f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
													 planet_rot_axis, object::AbstractObject::kFace };

		object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.9f, 0.9f, 0.75f } };

		// planet orbit
		constexpr float eccentricity = 0.0011f;
		constexpr float semimajor_axis = 20.0f;
		const     float longitude_of_acending_node = 0.0f;
		const     float argument_of_perihelion = angle(randomizer);
		constexpr float inclination = glm::radians(0.340f + 97.77f);
		const     float true_anomaly = angle(randomizer);
		constexpr float angular_velocity = 360.0f / 8.7f;
		object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
														 argument_of_perihelion, true_anomaly, angular_velocity };

		planet_system->addOrbit(planet, planet_orbit);
	}

	// OBERON
	{
		// planet physical object
		object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

		// planet
		float planet_radius{ 0.076f };
		object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
										  object::AbstractObject::kFace, glm::vec3{planet_radius} };

		glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ -360.0f / 13.5f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
															 planet_rot_axis, object::AbstractObject::kFace };

		object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.9f, 0.75f, 0.65f } };

		// planet orbit
		constexpr float eccentricity = 0.0014;
		constexpr float semimajor_axis = 25.0f;
		const     float longitude_of_acending_node = 0.0f;
		const     float argument_of_perihelion = angle(randomizer);
		constexpr float inclination = glm::radians(0.058f + 97.77f);
		const     float true_anomaly = angle(randomizer);
		constexpr float angular_velocity = 360.0f / 13.5f;
		object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
														 argument_of_perihelion, true_anomaly, angular_velocity };

		planet_system->addOrbit(planet, planet_orbit);
	}

	// planet system orbit
	constexpr float eccentricity = 0.04717f;
	constexpr float semimajor_axis = 250.0f;
	constexpr float longitude_of_acending_node = glm::radians(74.006f);
	constexpr float argument_of_perihelion = glm::radians(96.998857f);
	constexpr float inclination = glm::radians(0.99f);
	const float true_anomaly = angle(randomizer);
	constexpr float angular_velocity = 360.0f / 30689.0f;
	object::Orbit* planet_system_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															argument_of_perihelion, true_anomaly, angular_velocity };

	solar_system->addOrbit(planet_system, planet_system_orbit);
	}

	// NEPTUNE MOON SYSTEM
	{
	// planet system
	object::OrbitalSystem* planet_system = new object::OrbitalSystem{ };

	// planet physical object
	object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

	// planet
	float planet_radius{ 1.3f };
	object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
									  object::AbstractObject::kFace, glm::vec3{planet_radius} };

	glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(28.32f), object::AbstractObject::kFace);
	float planet_rot_angle{ angle(randomizer) };
	float planet_rot_angle_velocity{ 360.0f / 0.67f };
	object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
											 planet_rot_axis, object::AbstractObject::kFace };

	object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.65f, 0.75f, 1.0f } };

	planet_system->addOrbit(planet);

	// TRITON
	{
		// planet physical object
		object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

		// planet
		float planet_radius{ 0.14f };
		object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
										  object::AbstractObject::kFace, glm::vec3{planet_radius} };

		glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ -360.0f / 5.88f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
															 planet_rot_axis, object::AbstractObject::kFace };

		object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.75f, 0.7f, 0.65f } };

		// planet orbit
		constexpr float eccentricity = 0.000016;
		constexpr float semimajor_axis = 12.0f;
		const     float longitude_of_acending_node = angle(randomizer);
		const     float argument_of_perihelion = angle(randomizer);
		constexpr float inclination = glm::radians(129.6f);
		const     float true_anomaly = angle(randomizer);
		constexpr float angular_velocity = 360.0f / 5.88f;
		object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
														 argument_of_perihelion, true_anomaly, angular_velocity };

		planet_system->addOrbit(planet, planet_orbit);
	}

	// planet system orbit
	constexpr float eccentricity = 0.008678;
	constexpr float semimajor_axis = 300.0f;
	constexpr float longitude_of_acending_node = glm::radians(131.783f);
	constexpr float argument_of_perihelion = glm::radians(273.187f);
	constexpr float inclination = glm::radians(0.74f);
	const float true_anomaly = angle(randomizer);
	constexpr float angular_velocity = 360.0f / 60195.0f;
	object::Orbit* planet_system_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															argument_of_perihelion, true_anomaly, angular_velocity };

	solar_system->addOrbit(planet_system, planet_system_orbit);
	}

	// PLUTO MOON SYSTEM
	{
		// planet system
		object::OrbitalSystem* planet_system = new object::OrbitalSystem{ };

		// planet physical object
		object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

		// planet
		float planet_radius{ 0.12f };
		object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
										  object::AbstractObject::kFace, glm::vec3{planet_radius} };

		glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
		float planet_rot_angle{ angle(randomizer) };
		float planet_rot_angle_velocity{ 360.0f / 6.4f };
		object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
												 planet_rot_axis, object::AbstractObject::kFace };

		object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 1.0f, 0.95f, 0.75f } };

		// planet orbit

		{

			constexpr float eccentricity = 0.0f;
			constexpr float semimajor_axis = 0.2f;
			const     float longitude_of_acending_node = 0.0f;
			const     float argument_of_perihelion = 0.0f;
			constexpr float inclination = glm::radians(119.6f);
			constexpr float true_anomaly = 0.0f;
			constexpr float angular_velocity = 360.0f / 6.4f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);

		}

		// CHARON
		{
			// planet physical object
			object::AggregateObject* planet_object{ new object::AggregateObject{ new object::Object{ object::model::getDefaultSurface() } } };

			// planet
			float planet_radius{ 0.06f };
			object::AbstractObject planet_ao{ object::AbstractObject::kOrigo, object::AbstractObject::kUp,
											  object::AbstractObject::kFace, glm::vec3{planet_radius} };

			glm::vec3 planet_rot_axis = glm::rotate(object::AbstractObject::kUp, glm::radians(0.0f), object::AbstractObject::kFace);
			float planet_rot_angle{ angle(randomizer) };
			float planet_rot_angle_velocity{ 360.0f / 6.4f };
			object::AstronomicalObject planet_astro{ planet_ao, planet_object, planet_rot_angle, planet_rot_angle_velocity,
																 planet_rot_axis, object::AbstractObject::kFace };

			object::Planet* planet = new object::Planet{ planet_astro, glm::vec3{ 0.75f, 0.7f, 0.65f } };

			// planet orbit
			constexpr float eccentricity = 0.0002f;
			constexpr float semimajor_axis = 1.0f;
			const     float longitude_of_acending_node = 0.0f;
			const     float argument_of_perihelion = 0.0f;
			constexpr float inclination = glm::radians(119.6f);
			constexpr float true_anomaly = 180.0f;
			constexpr float angular_velocity = 360.0f / 6.4f;
			object::Orbit* planet_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
															 argument_of_perihelion, true_anomaly, angular_velocity };

			planet_system->addOrbit(planet, planet_orbit);
		}

		// planet system orbit
		constexpr float eccentricity = 0.2488f;
		constexpr float semimajor_axis = 400.0f;
		constexpr float longitude_of_acending_node = glm::radians(110.299f);
		constexpr float argument_of_perihelion = glm::radians(113.834f);
		constexpr float inclination = glm::radians(17.16f);
		const float true_anomaly = angle(randomizer);
		constexpr float angular_velocity = 360.0f / 90560.0f;
		object::Orbit* planet_system_orbit = new object::Orbit{ eccentricity, semimajor_axis, inclination, longitude_of_acending_node,
																argument_of_perihelion, true_anomaly, angular_velocity };

		solar_system->addOrbit(planet_system, planet_system_orbit);
	}

	return solar_system;
}

} // namespace generator
} // namespace simulation
} // namespace wanderers
