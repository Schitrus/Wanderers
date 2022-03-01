/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This program simulates a procedurally generated solarsystem.              *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* External Includes */
#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext.hpp"

/* Internal Includes */
#include "render_engine/camera.h"
#include "render_engine/space_renderer.h"
#include "render_engine/shader/shader_program.h"

#include "simulation/object/orbital_system.h"
#include "simulation/generator/solar_system_generator.h"

#include "control/controller.h"

/* STL Includes */
#include <iostream>

/* DEFINITIONS */
#define ENABLE_VERTICAL_SYNCHRONIZATION true

/*
 *  Setup window for rendering. 
 */
GLFWwindow* setupWindow() {
	const char* window_title{ "Wanderers" };
	constexpr int window_width{ 1920 };
	constexpr int window_height{ 1080 };

	GLFWwindow* window{ glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr) };

	glfwMakeContextCurrent(window);
	glfwSwapInterval(ENABLE_VERTICAL_SYNCHRONIZATION);

	return window;
}

/*
 *  Render loop:
 *  - Until exit is requested.
 *    - Proceed simulation.
 *    - Render.
 */
void renderLoop(OrbitalSystem* simulation, SpaceRenderer* renderer) {
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
 *  Program entry point:
 *  - Init graphics.
 *  - Setup simulation, render engine and controller.
 *  - Enter render loop until program exit is requested.
 */
int main(int argc, char** args) {
	// Init graphics.
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	GLFWwindow* window{ setupWindow() };

	gladLoadGL(glfwGetProcAddress);

	// Setup simulation.
	OrbitalSystem* solar_system{ generateSolarSystem(25.0f) };

	// Setup render engine.
	ShaderProgram shader{ "shaders/vertex.glsl", "shaders/fragment.glsl" };
	shader.link();

	Camera camera{ glm::vec3{0.0f, 16.0f, 0.0f}, 180.0f, -89.0f, 0.0f };
	SpaceRenderer* space_renderer = new SpaceRenderer{ shader, camera };

	// Setup controller
	Controller::initController(camera, *solar_system);

	// Render loop until exit is requested.
	renderLoop(solar_system, space_renderer);

	// Program exit.
	Controller::deinitController();

	delete space_renderer;
	delete solar_system;

	glfwDestroyWindow(window);
	glfwTerminate();
}