/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class controls the Camera.                                           *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_CONTROL_CAMERA_CONTROL_H_
#define WANDERERS_CONTROL_CAMERA_CONTROL_H_

/* Internal Includes */
#include "control/control_interface.h"
#include "render/camera.h"
#include "simulation/space_simulation.h"

namespace wanderers {
namespace control {

/*
 * Class for controlling the Camera.
 */
class CameraControl : public ControlInterface {
public:

	CameraControl(GLFWwindow* window, render::Camera* camera, simulation::SpaceSimulation* simulation);

	/* Interpret key changes and change state of the program. */
	void enactKeyTrigger(int key, double seconds);
	void enactKeyRelease(int key, double seconds);
	void enactKeyPress(int key, double seconds);
	/* Interpret cursor position changes and change state of the program. */
	void enactCursorPosition(glm::vec2 delta, double seconds);
	/* Interpret scroll offset changes and change state of the program. */
	void enactScrollOffset(glm::vec2 offset, double seconds);

protected:
	render::Camera* camera_;
	simulation::SpaceSimulation* simulation_;
};

} // namespace control
} // namespace wanderers

#endif // WANDERERS_CONTROL_CAMERA_CONTROL_H_
