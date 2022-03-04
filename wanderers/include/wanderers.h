/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Declarations of setup Wanderers program and running it.                   *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_WANDERERS_H_
#define WANDERERS_WANDERERS_H_

/* External Includes */
#include "glfw/glfw3.h"

/* Internal Includes */
#include "render/space_renderer.h"
#include "simulation/object/orbital_system.h"

namespace wanderers {

/* Setup window for rendering. */
static GLFWwindow* setupWindow();

/* Enter the simulations render loop. */
static void renderLoop(OrbitalSystem* simulation, render::SpaceRenderer* renderer);

/* Start running the Wanderers program. */
void run();

} // namespace wanderers

#endif // WANDERERS_WANDERERS_H_