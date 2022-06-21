/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class controls the Rendering.                                        *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_CONTROL_RENDER_CONTROL_H_
#define WANDERERS_CONTROL_RENDER_CONTROL_H_

/* Internal Includes */
#include "control/control_interface.h"
#include "render/space_renderer.h"

namespace wanderers {
namespace control {

/*
 * Class for controlling the rendering.
 */
class RenderControl : public ControlInterface {
public:
	RenderControl(GLFWwindow* window, render::SpaceRenderer* renderer);

	/* Interpret key changes and change state of the program. */
	void enactKeyTrigger(int key, double seconds);
	void enactKeyRelease(int key, double seconds);
	void enactKeyPress(int key, double seconds);
	/* Interpret cursor position changes and change state of the program. */
	void enactCursorPosition(glm::vec2 delta, double seconds);
	/* Interpret scroll offset changes and change state of the program. */
	void enactScrollOffset(glm::vec2 offset, double seconds);

protected:
	render::SpaceRenderer* renderer_;

};

} // namespace control
} // namespace wanderers

#endif // WANDERERS_CONTROL_RENDER_CONTROL_H_
