#ifndef SOLAR_SYSTEM_GENERATOR_H
#define SOLAR_SYSTEM_GENERATOR_H

#include "OrbitalSystem.h"
#include "Solar.h"
#include "Orbit.h"
#include "Planet.h"

OrbitalSystem* generateSolarSystem(float radius);

Solar* generateSolar(float radius);

Orbit* generatePlanetSystem(float radius, float orbit_radius);

Planet* generatePlanet(float radius);

#endif // SOLAR_SYSTEM_GENERATOR_H
