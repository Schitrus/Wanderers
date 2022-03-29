
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of setup Wanderers program and running it.                 *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "wanderers.h"

/* External Includes */
#include "glad/gl.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext.hpp"

/* Internal Includes */
#include "render/camera.h"
#include "render/shader/shader_program.h"
#include "simulation/space_simulation.h"
#include "simulation/object/stars.h"
#include "simulation/generator/solar_system_generator.h"
#include "control/controller.h"

/* STL Includes */
#include <random>
#include <chrono>

namespace wanderers {

/*  
 *  setupWindow:
 *  - Create window.
 *  - Make window the current context.
 */
GLFWwindow* setupWindow() {
	const char* window_title{ "Wanderers" };
	constexpr int window_width{ 1920 };
	constexpr int window_height{ 1080 };

	GLFWwindow* window{ glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr) };

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	return window;
}

/*  
 *  renderLoop:
 *  - Until exit is requested:
 *    - Proceed simulation.
 *    - Render.
 */
void renderLoop(simulation::SpaceSimulation* simulation, render::SpaceRenderer* renderer) {
	double last_time{ glfwGetTime() };

	while (!glfwWindowShouldClose(glfwGetCurrentContext())) {
		double dt = glfwGetTime() - last_time;
		last_time += dt;

		simulation->elapseTime(dt);

		renderer->preRender();
		renderer->render(simulation);
		renderer->postRender();
	}
}

/*
 *  run:
 *  - Init graphics.
 *  - Setup simulation, render engine and controller.
 *  - Enter render loop until program exit is requested.
 */
void run() {
	// Init graphics.
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_SAMPLES, 16);
	
	GLFWwindow* window{ setupWindow() };
	
	gladLoadGL(glfwGetProcAddress);
	
	// Setup simulation.
	render::Camera* camera{ new render::Camera{glm::vec3{0.0f, 16.0f, 0.0f}, glm::vec3{0.0f, 0.0f,-1.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, 60.0f, 1.0f, 0.01f, 1000.0f } };
	simulation::SpaceSimulation* space_simulation = new simulation::SpaceSimulation{camera};
	
	// Setup render engine.
	render::shader::ShaderProgram* shader{ new render::shader::ShaderProgram{"shaders/vertex.glsl", "shaders/fragment.glsl"} };
	shader->link();
	
	render::SpaceRenderer* space_renderer = new render::SpaceRenderer{ shader, camera };
	
	// Setup controller
	control::Controller::initController(camera, space_simulation);
	
	// Render loop until exit is requested.
	renderLoop(space_simulation, space_renderer);
	
	// Program exit.
	control::Controller::deinitController();
	
	delete space_renderer;
	delete space_simulation;
	
	glfwDestroyWindow(window);
	glfwTerminate();
}

} // namespace wanderers