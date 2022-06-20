/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class controls the Simulation.                                           *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_CONTROL_SIMULATION_CONTROL_H_
#define WANDERERS_CONTROL_SIMULATION_CONTROL_H_

/* Internal Includes */
#include "control/control_interface.h"
#include "simulation/space_simulation.h"

namespace wanderers {
namespace control {

/*
 * Class for controlling the simulation.
 */
class SimulationControl : public ControlInterface {
public:
	SimulationControl(GLFWwindow* window, simulation::SpaceSimulation* simulation);

	/* Interpret key changes and change state of the program. */
	void enactKeyTrigger(int key, double seconds);
	void enactKeyRelease(int key, double seconds);
	void enactKeyPress(int key, double seconds);
	/* Interpret cursor position changes and change state of the program. */
	void enactCursorPosition(glm::vec2 delta, double seconds);
	/* Interpret scroll offset changes and change state of the program. */
	void enactScrollOffset(glm::vec2 offset, double seconds);

protected:
	simulation::SpaceSimulation* simulation_;

};

} // namespace control
} // namespace wanderers

#endif // WANDERERS_CONTROL_SIMULATION_CONTROL_H_
