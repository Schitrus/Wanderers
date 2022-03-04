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
#include "render_engine/camera.h"
#include "render_engine/shader/shader_program.h"
#include "simulation/object/orbital_system.h"
#include "simulation/generator/solar_system_generator.h"
#include "control/controller.h"

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
void renderLoop(OrbitalSystem* simulation, render_engine::SpaceRenderer* renderer) {
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
	
	GLFWwindow* window{ setupWindow() };
	
	gladLoadGL(glfwGetProcAddress);
	
	// Setup simulation.
	OrbitalSystem* solar_system{ generateSolarSystem(25.0f) };
	
	// Setup render engine.
	render_engine::shader::ShaderProgram shader{ "shaders/vertex.glsl", "shaders/fragment.glsl" };
	shader.link();
	
	render_engine::Camera camera{ glm::vec3{0.0f, 16.0f, 0.0f}, 180.0f, -89.0f, 0.0f };
	render_engine::SpaceRenderer* space_renderer = new render_engine::SpaceRenderer{ shader, camera };
	
	// Setup controller
	control::Controller::initController(camera, *solar_system);
	
	// Render loop until exit is requested.
	renderLoop(solar_system, space_renderer);
	
	// Program exit.
	control::Controller::deinitController();
	
	delete space_renderer;
	delete solar_system;
	
	glfwDestroyWindow(window);
	glfwTerminate();
}

} // namespace wanderers