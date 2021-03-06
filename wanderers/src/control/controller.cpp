/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Controller class and input callbacks.               *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "control/controller.h"

/* Internal Includes */
#include "control/camera_control.h"
#include "control/simulation_control.h"
#include "control/render_control.h"

/* STL Includes */
#include <thread>

namespace wanderers {
namespace control {

/*
 * Controller Constructor:
 * - Set input callbacks.
 * - Start controller thread.
 */
Controller::Controller(GLFWwindow* window, render::Camera* camera, simulation::SpaceSimulation* simulation, render::SpaceRenderer* renderer) : ControlInterface{ window },
                                                             pressed_keys_{ std::size_t{kNumKeys} },
                                                             released_keys_{ std::size_t{kNumKeys} },
	                                                         cursor_position_{} {

	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	cursor_position_.setPosition(glm::vec2{0.0f, 0.0f});
	glfwSetCursorPos(window_, 0.0f, 0.0f);

	glfwSetScrollCallback(window_, scrollCallback);
	glfwSetCursorPosCallback(window_, cursorPositionCallback);
	glfwSetKeyCallback(window_, keyCallback);

	controls_.push_back(new CameraControl{ window, camera, simulation });
	controls_.push_back(new SimulationControl{ window, simulation });
	controls_.push_back(new RenderControl{ window, renderer });

	should_stop_ = false;
	std::thread controller_thread{&Controller::runController, this};
	controller_thread.detach();
}

/*
 * Controller Destructor:
 * - Stop thread and reset input callbacks.
 */
Controller::~Controller() { 
	std::lock_guard<std::mutex> guard(controller_mutex_);
	should_stop_ = true; 
	glfwSetKeyCallback(window_, nullptr); 
	glfwSetCursorPosCallback(window_, nullptr);
	glfwSetScrollCallback(window_, nullptr);
}

/*
 * Controller initController: 
 * - Create Controller singleton if not created.
 */
void Controller::initController(render::Camera* camera, simulation::SpaceSimulation* simulation, render::SpaceRenderer* renderer) {
	if (controller_singleton_ == nullptr)
		controller_singleton_ = new Controller{ glfwGetCurrentContext(), camera, simulation, renderer};
}

/*
 * Controller deinitController:
 * - Destroy Controller singleton if it exists.
 */
void Controller::deinitController() {
	if (controller_singleton_ != nullptr)
		delete controller_singleton_;
}

Controller* Controller::getController() {
	return controller_singleton_;
}

/* CursorPosition Constructor. */
Controller::CursorPosition::CursorPosition() : position_{ 0.0f }, delta_{ 0.0f } {}

/*
 * CursorPosition updatePosition:
 * - Set position and update delta.
 */
void Controller::CursorPosition::updatePosition(glm::vec2 position) {
	std::lock_guard<std::mutex> guard(cursor_position_mutex_);
	delta_ += position - position_;

	position_ = position;
}

void Controller::CursorPosition::setPosition(glm::vec2 position) {
	std::lock_guard<std::mutex> guard(cursor_position_mutex_);
	position_ = position;
}

glm::vec2 Controller::CursorPosition::getPosition() {
	std::lock_guard<std::mutex> guard(cursor_position_mutex_);
	return position_;
}

/*
 * CursorPosition fetchDelta:
 * - Return delta and reset variable.
 */
glm::vec2 Controller::CursorPosition::fetchDelta() {
	std::lock_guard<std::mutex> guard(cursor_position_mutex_);
	glm::vec2 delta{ delta_ };
	delta_ = glm::vec2{ 0.0f };
	return delta;
}

/*
 * Controller runController:
 * - Until controller singleton is destroyed:
 *   - Handle controls.
 */
void Controller::runController() {
	double last_time{ glfwGetTime() };
	while (!should_stop_) {
		double dt = glfwGetTime() - last_time;
		last_time += dt;
		handleControls(dt);
		
		std::this_thread::sleep_for(std::chrono::milliseconds{ 10 }); // make dynamic
	}
}

/*
 * Controller handleControls:
 * - Enact changes in cursor position.
 * - Enact changes in key changes.
 */
void Controller::handleControls(double seconds) {
	std::lock_guard<std::mutex> guard(controller_mutex_);
	enactCursorPosition(cursor_position_.fetchDelta(), seconds);

	enactScrollOffset(scroll_offset_, seconds);
	scroll_offset_ = glm::vec2{ 0.0f };

	for (int triggered_key : triggered_keys_)
		enactKeyTrigger(triggered_key, seconds);

	triggered_keys_.clear(); // might cause bugs

	for (int released_key : released_keys_)
		enactKeyRelease(released_key, seconds);

	released_keys_.clear(); // might cause bugs

	for (int pressed_key : pressed_keys_)
		enactKeyPress(pressed_key, seconds);
}

void Controller::enactKeyTrigger(int key, double seconds) {
	for (ControlInterface* control : controls_)
		control->enactKeyTrigger(key, seconds);
}

void Controller::enactKeyRelease(int key, double seconds) {
	for (ControlInterface* control : controls_)
		control->enactKeyRelease(key, seconds);
}

void Controller::enactKeyPress(int key, double seconds) {
	switch (key) {
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window_, GLFW_TRUE);
		should_stop_ = true;
	}
	for (ControlInterface* control : controls_)
		control->enactKeyPress(key, seconds);
}

void Controller::enactCursorPosition(glm::vec2 delta, double seconds) {
	for (ControlInterface* control : controls_)
		control->enactCursorPosition(delta, seconds);
}

void Controller::enactScrollOffset(glm::vec2 offset, double seconds) {
	for (ControlInterface* control : controls_)
		control->enactScrollOffset(offset, seconds);
}

/*
 * Controller updateKey:
 * - If key is triggered insert to triggered keys.
 * - If key is pressed insert to pressed keys.
 * - If key is released:
 *   - Remove from pressed keys.
 *   - Insert to released keys.
 */
void Controller::updateKey(int key, int action) { 
	std::lock_guard<std::mutex> guard(controller_mutex_);
	if (action == GLFW_PRESS) {
		if (pressed_keys_.insert(key).second)
			triggered_keys_.insert(key);
	}
	else if (action == GLFW_RELEASE) {
		pressed_keys_.erase(key);
		released_keys_.insert(key);
	}
}

void Controller::updateCursorPosition(double x_position, double y_position) {
	std::lock_guard<std::mutex> guard(controller_mutex_);
	cursor_position_.updatePosition(glm::vec2{ x_position, y_position });
}

void Controller::updateScrollOffset(double x_offset, double y_offset) {
	std::lock_guard<std::mutex> guard(controller_mutex_);
	scroll_offset_ = glm::vec2{ x_offset, y_offset };
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	controller_singleton_->updateKey(key, action);
}

static void cursorPositionCallback(GLFWwindow* window, double x_position, double y_position) {
	controller_singleton_->updateCursorPosition(x_position, y_position);
}

static void scrollCallback(GLFWwindow* window, double x_offset, double y_offset) {
	controller_singleton_->updateScrollOffset(x_offset, y_offset);
}

} // namsepace wanderers
} // namsepace control
