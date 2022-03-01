#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext.hpp"

#include <iostream>

#include "render_engine/camera.h"
#include "render_engine/space_renderer.h"
#include "render_engine/shader/shader_program.h"

#include "simulation/object/orbital_system.h"
#include "simulation/generator/solar_system_generator.h"

#include "control/controller.h"

#define ENABLE_VERTICAL_SYNCHRONIZATION true

GLFWwindow* setupWindow() {
	const char* window_title{ "Wanderers" };
	constexpr int window_width{ 1920 };
	constexpr int window_height{ 1080 };

	GLFWwindow* window{ glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr) };

	glfwMakeContextCurrent(window);
	glfwSwapInterval(ENABLE_VERTICAL_SYNCHRONIZATION);

	return window;
}

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

int main(int argc, char** args) {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	GLFWwindow* window{ setupWindow() };

	gladLoadGL(glfwGetProcAddress);

	Camera camera{ glm::vec3{0.0f, 16.0f, 0.0f}, 180.0f, -89.0f, 0.0f };

	ShaderProgram shader{ "shaders/vertex.glsl", "shaders/fragment.glsl" };
	shader.link();

	SpaceRenderer* space_renderer = new SpaceRenderer{ shader, camera };

	OrbitalSystem* solar_system{ generateSolarSystem(25.0f) };

	Controller::initController(camera, *solar_system);

	renderLoop(solar_system, space_renderer);

	Controller::deinitController();

	delete space_renderer;
	delete solar_system;

	glfwDestroyWindow(window);
	glfwTerminate();
}