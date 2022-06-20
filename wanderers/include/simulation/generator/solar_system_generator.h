/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Functions for generating a Solarsystem and parts of it.                   *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_GENERATOR_SOLAR_SYSTEM_GENERATOR_H_
#define WANDERERS_SIMULATION_GENERATOR_SOLAR_SYSTEM_GENERATOR_H_

/* Internal Includes */
#include "simulation/object/planet.h"
#include "simulation/object/solar.h"
#include "simulation/object/orbit.h"
#include "simulation/object/orbital_system.h"

namespace wanderers {
namespace simulation {
namespace generator {

/* Generates the solarsystem. */
object::OrbitalSystem* generateTheSolarSystem();

/* Generates a solarsystem that stretches out as far out as the radius. */
object::OrbitalSystem* generateSolarSystem(float radius);

/* Generates a orbit within the lower and upper radius, sets angulare velocity accordingly to the velocity factor. */
object::Orbit* generateOrbit(float lower_radius, float upper_radius, float velocity_factor);

/* Generates a solar that has the given radius. */
object::Solar* generateSolar(float radius);

/* Generates a planet system that have the maximum size of the radius. */
object::OrbitalSystem* generatePlanetSystem(float radius, float orbit_radius);

/* Generates a planet that has the given radius. */
object::Planet* generatePlanet(float radius);

} // namespace generator
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_GENERATOR_SOLAR_SYSTEM_GENERATOR_H_
