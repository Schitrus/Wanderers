#include "Controller.h"

#include <thread>

#include <iostream>

#include "SolarSystemGenerator.h"

Controller::Controller(GLFWwindow* window, Camera& camera, OrbitalSystem& simulation) : window_{ window }, camera_{ camera }, simulation_{ simulation },
                                                             pressed_keys_{ std::size_t{kNumKeys} },
                                                             released_keys_{ std::size_t{kNumKeys} },
	                                                         cursor_position_{} {
	glfwSetKeyCallback(window_, keyCallback);

	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	cursor_position_.setPosition(glm::vec2{0.0f, 0.0f});
	glfwSetCursorPos(window_, 0.0f, 0.0f);
	glfwSetCursorPosCallback(window_, cursorPositionCallback);

	should_stop_ = false;
	std::thread controller_thread{&Controller::runController, this};
	controller_thread.detach();
}

Controller::~Controller() { 
	should_stop_ = true; 
	glfwSetKeyCallback(window_, nullptr);
}

void Controller::initController(Camera& camera, OrbitalSystem& simulation) {
	if (controller_singleton_ == nullptr)
		controller_singleton_ = new Controller{ glfwGetCurrentContext(), camera, simulation};
}

void Controller::deinitController() {
	if (controller_singleton_ != nullptr)
		delete controller_singleton_;
}

Controller* Controller::getController() {
	return controller_singleton_;
}

void Controller::runController() {
	double last_time{ glfwGetTime() };
	while (!should_stop_) {
		double dt = glfwGetTime() - last_time;
		last_time += dt;
		handleControls(dt);
		
		std::this_thread::sleep_for(std::chrono::milliseconds{ 10 });
	}
}

void Controller::handleControls(double seconds) {
	enactCursorPosition(seconds);

	for (int triggered_key : triggered_keys_)
		enactKeyTrigger(triggered_key, seconds);

	triggered_keys_.clear(); // might cause bugs

	for (int released_key : released_keys_)
		enactKeyRelease(released_key, seconds);

	released_keys_.clear(); // might cause bugs

	for (int pressed_key : pressed_keys_)
		enactKeyPress(pressed_key, seconds);
}

void Controller::enactKeyPress(int key, double seconds) {
	switch (key) {
	case GLFW_KEY_W:
		camera_.move(static_cast<float>(4.0 * seconds) * glm::vec3{ 0.0f, 0.0f,-1.0f });
		break;
	case GLFW_KEY_S:
		camera_.move(static_cast<float>(4.0 * seconds) * glm::vec3{ 0.0f, 0.0f, 1.0f });
		break;
	case GLFW_KEY_A:
		camera_.move(static_cast<float>(4.0 * seconds) * glm::vec3{-1.0f, 0.0f, 0.0f });
		break;
	case GLFW_KEY_D:
		camera_.move(static_cast<float>(4.0 * seconds) * glm::vec3{ 1.0f, 0.0f, 0.0f });
		break;
	case GLFW_KEY_LEFT_SHIFT:
		camera_.move(static_cast<float>(4.0 * seconds) * glm::vec3{ 0.0f, 1.0f, 0.0f });
		break;
	case GLFW_KEY_LEFT_CONTROL:
		camera_.move(static_cast<float>(4.0 * seconds) * glm::vec3{ 0.0f,-1.0f, 0.0f });
		break;
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window_, GLFW_TRUE);
		should_stop_ = true;
	}
}

void Controller::enactKeyTrigger(int key, double seconds) {
	switch (key) {
	case GLFW_KEY_R:
		simulation_ = generateSolarSystem(25.0f);
	case GLFW_KEY_SPACE:
		simulation_.isPaused() ? simulation_.unpause() : simulation_.pause();
		break;
	}
}

void Controller::enactKeyRelease(int key, double seconds) {

}

void Controller::enactCursorPosition(double seconds) {
	glm::vec2 delta{ cursor_position_.pullDelta() };

	int width, height;
	glfwGetFramebufferSize(window_, &width, &height);

	camera_.turnYaw(-delta.x/10.0f);
	camera_.turnPitch(-delta.y/10.0f);
}

void Controller::updateKey(int key, int action) { 
	if (action == GLFW_PRESS) {
		if (pressed_keys_.insert(key).second)
			triggered_keys_.insert(key);
	}
	else if (action == GLFW_RELEASE) {
		pressed_keys_.erase(key);
		released_keys_.insert(key);
	}
}

Controller::CursorPosition::CursorPosition() : position_{0.0f}, delta_{0.0f} {}

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
glm::vec2 Controller::CursorPosition::pullDelta() {
	std::lock_guard<std::mutex> guard(cursor_position_mutex_);
	glm::vec2 delta{ delta_ };
	delta_ = glm::vec2{ 0.0f };
	return delta;
}

void Controller::updateCursorPosition(double x_position, double y_position) {
	cursor_position_.updatePosition(glm::vec2{ x_position, y_position });
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	controller_singleton_->updateKey(key, action);
}

static void cursorPositionCallback(GLFWwindow* window, double x_position, double y_position) {
	controller_singleton_->updateCursorPosition(x_position, y_position);
}