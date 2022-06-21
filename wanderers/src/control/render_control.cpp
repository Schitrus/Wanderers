/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Render control class.                               *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "control/render_control.h"

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

RenderControl::RenderControl(GLFWwindow* window, render::SpaceRenderer* renderer) : ControlInterface{ window }, renderer_{ renderer } {}

void RenderControl::enactKeyTrigger(int key, double seconds) {
	switch (key) {
	// O: Show/hide orbits.
	case GLFW_KEY_O:
		renderer_->setShowOrbits(!renderer_->getShowOrbits());
		break;
	}

}

void RenderControl::enactKeyRelease(int key, double seconds) {
	// No function for now.
}

void RenderControl::enactKeyPress(int key, double seconds) {
	// No function for now.
}

void RenderControl::enactCursorPosition(glm::vec2 delta, double seconds) {
	// No function for now.
}

void RenderControl::enactScrollOffset(glm::vec2 offset, double seconds) {
	// No function for now
}

} // namespace control
} // namespace wanderers
