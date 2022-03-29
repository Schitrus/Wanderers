/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Camera control class.                                  *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "control/camera_control.h"

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

CameraControl::CameraControl(GLFWwindow* window, render::Camera* camera) : ControlInterface{ window }, camera_{ camera }, mode_{ kDefaultMode }, focus_id_{ 0 } {}
CameraControl::CameraControl(GLFWwindow* window, render::Camera* camera, Mode mode) : ControlInterface{ window }, camera_ { camera }, mode_{ mode }, focus_id_{ 0 } {}


CameraControl::Mode CameraControl::getMode() {
    return mode_;
}

void CameraControl::setMode(CameraControl::Mode mode) {
    mode_ = mode;
}

CameraControl::Mode CameraControl::cycleMode() {
    mode_ = static_cast<CameraControl::Mode>((static_cast<int>(mode_) + 1) % static_cast<int>(CameraControl::Mode::Count));
    return mode_;
}

unsigned int CameraControl::getFocusId() {
    return focus_id_;
}

void CameraControl::setFocusId(unsigned int focus_id) {
    focus_id_ = focus_id;
}

void CameraControl::enactKeyTrigger(int key, double seconds) {
	switch (key) {
	case GLFW_KEY_V:
		cycleMode();
		break;
	case GLFW_KEY_TAB:
		//setFocusId((focus_id_ + 1) % simulation_.getSize(simulation_.getSolarSystems().at(0)));
		break;
	}
}

void CameraControl::enactKeyRelease(int key, double seconds) {
	// No function for now.
}

void CameraControl::enactKeyPress(int key, double seconds) {
	switch (key) {
	case GLFW_KEY_W:
		camera_->move(static_cast<float>(4.0 * seconds) * glm::vec3{ 0.0f, 0.0f,-1.0f });
		break;
	case GLFW_KEY_S:
		camera_->move(static_cast<float>(4.0 * seconds) * glm::vec3{ 0.0f, 0.0f, 1.0f });
		break;
	case GLFW_KEY_A:
		camera_->move(static_cast<float>(4.0 * seconds) * glm::vec3{ -1.0f, 0.0f, 0.0f });
		break;
	case GLFW_KEY_D:
		camera_->move(static_cast<float>(4.0 * seconds) * glm::vec3{ 1.0f, 0.0f, 0.0f });
		break;
	case GLFW_KEY_E:
		camera_->turnRoll(static_cast<float>(90.0 * seconds));
		break;
	case GLFW_KEY_Q:
		camera_->turnRoll(static_cast<float>(-90.0 * seconds));
		break;
	case GLFW_KEY_LEFT_SHIFT:
		camera_->move(static_cast<float>(4.0 * seconds) * glm::vec3{ 0.0f, 1.0f, 0.0f });
		break;
	case GLFW_KEY_LEFT_CONTROL:
		camera_->move(static_cast<float>(4.0 * seconds) * glm::vec3{ 0.0f,-1.0f, 0.0f });
		break;
	}
}

void CameraControl::enactCursorPosition(glm::vec2 delta, double seconds) {
	int width, height;
	glfwGetFramebufferSize(window_, &width, &height);

	camera_->turnYaw(-delta.x / 10.0f);
	camera_->turnPitch(-delta.y / 10.0f);
}

void CameraControl::enactScrollOffset(glm::vec2 offset, double seconds) {

}

} // namespace control
} // namespace wanderers
