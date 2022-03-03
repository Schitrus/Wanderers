#ifndef WANDERERS_SIMULATION_GENERATOR_SOLAR_SYSTEM_GENERATOR_H_
#define WANDERERS_SIMULATION_GENERATOR_SOLAR_SYSTEM_GENERATOR_H_

/* Internal Includes */
#include "simulation/object/planet.h"
#include "simulation/object/solar.h"
#include "simulation/object/orbit.h"
#include "simulation/object/orbital_system.h"

OrbitalSystem* generateSolarSystem(float radius);

Solar* generateSolar(float radius);

Orbit* generatePlanetSystem(float radius, float orbit_radius);

Planet* generatePlanet(float radius);

#endif // WANDERERS_SIMULATION_GENERATOR_SOLAR_SYSTEM_GENERATOR_H_
