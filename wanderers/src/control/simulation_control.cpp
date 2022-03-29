/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Simulation control class.                           *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "control/simulation_control.h"

/* External Includes */
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/vector_angle.hpp"

/* STL Includes */
#include <algorithm>

#include <iostream>

namespace wanderers {
namespace control {

SimulationControl::SimulationControl(GLFWwindow* window, simulation::SpaceSimulation* simulation) : ControlInterface{ window }, simulation_{ simulation } {}

void SimulationControl::enactKeyTrigger(int key, double seconds) {
	switch (key) {
	case GLFW_KEY_SPACE:
		simulation_->isPaused() ? simulation_->unpause() : simulation_->pause();
		break;
	}
}

void SimulationControl::enactKeyRelease(int key, double seconds) {
	// No function for now.
}

void SimulationControl::enactKeyPress(int key, double seconds) {
}

void SimulationControl::enactCursorPosition(glm::vec2 delta, double seconds) {
}

void SimulationControl::enactScrollOffset(glm::vec2 offset, double seconds) {
	simulation_->setSpeed(simulation_->getSpeed() * pow(1.01f, offset.y));
}

} // namespace control
} // namespace wanderers
