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

CameraControl::CameraControl(GLFWwindow* window, render::Camera* camera, simulation::SpaceSimulation* simulation) : ControlInterface{ window }, camera_{ camera }, simulation_{ simulation } {}


void CameraControl::enactKeyTrigger(int key, double seconds) {
	switch (key) {
	case GLFW_KEY_V:
		camera_->withMutext([this]() {camera_->cycleCameraMode(); });
		break;
	case GLFW_KEY_TAB:
		simulation_->cycleCameraFocusId();
		break;
	}
}

void CameraControl::enactKeyRelease(int key, double seconds) {
	// No function for now.
}

void CameraControl::enactKeyPress(int key, double seconds) {
	camera_->lock();
	switch (key) {
	case GLFW_KEY_W:
		camera_->move(static_cast<float>(4.0 * seconds) * glm::vec3{ 0.0f, 0.0f, 1.0f });
		break;
	case GLFW_KEY_S:
		camera_->move(static_cast<float>(4.0 * seconds) * glm::vec3{ 0.0f, 0.0f,-1.0f });
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
	camera_->unlock();
}

void CameraControl::enactCursorPosition(glm::vec2 delta, double seconds) {
	int width, height;
	glfwGetFramebufferSize(window_, &width, &height);

	camera_->lock();
	if (camera_->getCameraMode() == simulation::object::CameraObject::CameraMode::Center) {
		camera_->move(-delta.x / 100.0f * glm::vec3{ 1.0f, 0.0f, 0.0f });
		camera_->move(-delta.y / 100.0f * glm::vec3{ 0.0f, 1.0f, 0.0f });
	} else {
		camera_->turnYaw(-delta.x / 10.0f);
		camera_->turnPitch(-delta.y / 10.0f);
	}
	camera_->unlock();
}

void CameraControl::enactScrollOffset(glm::vec2 offset, double seconds) {

}

} // namespace control
} // namespace wanderers
