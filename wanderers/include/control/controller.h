#ifndef WANDERERS_CONTROL_CONTROLLER_H_
#define WANDERERS_CONTROL_CONTROLLER_H_

#include "glfw/glfw3.h"

#include "render_engine/camera.h"

#include "simulation/object/orbital_system.h"

#include <set>
#include <atomic>
#include <mutex>

class Controller {
public:
	static void initController(Camera& camera, OrbitalSystem& simulation);

	static void deinitController();

	static Controller* getController();

	~Controller();
private:

	GLFWwindow* window_;

	Camera& camera_;

	OrbitalSystem& simulation_;

	static constexpr int kNumKeys{ GLFW_KEY_LAST };
	std::set<int> pressed_keys_;

	std::set<int> released_keys_;
	std::set<int> triggered_keys_;

	class CursorPosition {
	public:
		CursorPosition();

		void updatePosition(glm::vec2 position);

		void setPosition(glm::vec2 position);

		glm::vec2 getPosition();

		glm::vec2 pullDelta();
	private:
		std::mutex cursor_position_mutex_;

		glm::vec2 position_;

		glm::vec2 delta_;
	} cursor_position_;

	std::atomic<bool> should_stop_;

	void updateKey(int key, int action);
	void updateCursorPosition(double x_position, double y_position);

	Controller(GLFWwindow* window, Camera& camera, OrbitalSystem& simulation);

	void runController();

	void handleControls(double seconds);

	void enactKeyPress(int key, double seconds);
	void enactKeyRelease(int key, double seconds);
	void enactKeyTrigger(int key, double seconds);

	void enactCursorPosition(double seconds);

	friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	friend void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

};

static Controller* controller_singleton_;

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

static void cursorPositionCallback(GLFWwindow* window, double x_position, double y_position);

#endif // WANDERERS_CONTROL_CONTROLLER_H_
