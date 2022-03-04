/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class controls the simulation with input from the keyboard and       *
 *   mouse. The controller handles the input with its own thread.            *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_CONTROL_CONTROLLER_H_
#define WANDERERS_CONTROL_CONTROLLER_H_

/* External Includes */
#include "glfw/glfw3.h"

/* Internal Includes */
#include "render_engine/camera.h"
#include "simulation/object/orbital_system.h"

/* STL Includes */
#include <set>
#include <atomic>
#include <mutex>

namespace wanderers {
namespace control {

/* 
 * Class for which to store key and mouse input changes and interpret them
 *   in the context of the program.
 */
class Controller {
public:
	/* Intitializes the controller singleton and start controller thread. */
	static void initController(render_engine::Camera& camera, OrbitalSystem& simulation);
	/* Deinitializes the controller singleton and stops the controller thread. */
	static void deinitController();

	/* Getter for the controller singleton. */
	static Controller* getController();

	~Controller();
private:
	/* Window for which to interpret input from. */
	GLFWwindow* window_;

	render_engine::Camera& camera_;
	OrbitalSystem& simulation_;

	/* Boolean for stopping the controller thread. */
	std::atomic<bool> should_stop_;

	/* Class to store cursor position change. */
	class CursorPosition {
	public:
		CursorPosition();

		/* Set cursor position and update cursor change. */
		void updatePosition(glm::vec2 position);
		/* Set cursor position without updating cursor change. */
		void setPosition(glm::vec2 position);

		glm::vec2 getPosition();

		/* Get the change in cursor position since last fetch. */
		glm::vec2 fetchDelta();
	private:
		std::mutex cursor_position_mutex_;

		glm::vec2 position_;

		/* Change in cursor position. */
		glm::vec2 delta_;
	} cursor_position_; // Cursor position object.

	/* Key changes. */
	std::set<int> pressed_keys_;
	std::set<int> released_keys_;
	std::set<int> triggered_keys_;

	/* Constructor for singleton. */
	Controller(GLFWwindow* window, render_engine::Camera& camera, OrbitalSystem& simulation);

	/* Run the controller handling loop. */
	void runController();

	/* Handle changes to control inputs. */
	void handleControls(double seconds);

	/* Store key and mouse changes for interpretation. */
	void updateKey(int key, int action);
	void updateCursorPosition(double x_position, double y_position);

	/* Interpret key changes and change state of the program. */
	void enactKeyTrigger(int key, double seconds);
	void enactKeyRelease(int key, double seconds);
	void enactKeyPress(int key, double seconds);
	/* Interpret cursor position changes and change state of the program. */
	void enactCursorPosition(double seconds);

	/* Number of possible different key presses. */
	static constexpr int kNumKeys{ GLFW_KEY_LAST };

	friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	friend void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
};

/* The controller singleton. */
static Controller* controller_singleton_;

/* Raw key callback. */
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

/* Raw cursor position callback. */
static void cursorPositionCallback(GLFWwindow* window, double x_position, double y_position);

} // namespace control
} // namespace wanderers

#endif // WANDERERS_CONTROL_CONTROLLER_H_
