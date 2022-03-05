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

namespace simulation{
namespace generator{

/* Generates a solarsystem that stretches out as far out as the radius. */
object::OrbitalSystem* generateSolarSystem(float radius);

/* Generates a solar that has the given radius. */
object::Solar* generateSolar(float radius);

/* Generates a planet system that have the maximum size of the radius. */
object::Orbit* generatePlanetSystem(float radius, float orbit_radius);

/* Generates a planet that has the given radius. */
object::Planet* generatePlanet(float radius);

} // namespace generator
} // namespace simulation

#endif // WANDERERS_SIMULATION_GENERATOR_SOLAR_SYSTEM_GENERATOR_H_
