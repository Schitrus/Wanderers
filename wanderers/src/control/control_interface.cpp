/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Controller class and input callbacks.               *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "control/control_interface.h"

/* STL Includes */
#include <thread>

namespace wanderers {
namespace control {

/*
 * Controller Constructor:
 * - Set input callbacks.
 * - Start controller thread.
 */
ControlInterface::ControlInterface(GLFWwindow* window) : window_{ window } {}

} // namsepace wanderers
} // namsepace control
