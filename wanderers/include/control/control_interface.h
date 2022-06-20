/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class interface is for enacting input changes.                       *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_CONTROL_CONTROL_INTERFACE_H_
#define WANDERERS_CONTROL_CONTROL_INTERFACE_H_

/* External Includes */
#include "glfw/glfw3.h"
#include "glm/glm.hpp"

namespace wanderers {
namespace control {

/* 
 * Class Interface for which to enact input control.
 */
class ControlInterface {
public:
	ControlInterface(GLFWwindow* window_);

	/* Interpret key changes and change state of the program. */
	virtual void enactKeyTrigger(int key, double seconds) = 0;
	virtual void enactKeyRelease(int key, double seconds) = 0;
	virtual void enactKeyPress(int key, double seconds) = 0;
	/* Interpret cursor position changes and change state of the program. */
	virtual void enactCursorPosition(glm::vec2 delta, double seconds) = 0;
	/* Interpret scroll offset changes and change state of the program. */
	virtual void enactScrollOffset(glm::vec2 offset, double seconds) = 0;
protected:
	/* Window for which to interpret input from. */
	GLFWwindow* window_;
};

} // namespace control
} // namespace wanderers

#endif // WANDERERS_CONTROL_CONTROLLER_H_

