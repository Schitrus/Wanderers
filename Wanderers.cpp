#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <math.h>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(int argc, char** args) {
	glfwInit();

	constexpr int window_width{ 1920 };
	constexpr int window_height{ 1080 };

	const char* window_title = "Wanderers";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr);

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);

	int render_width, render_height;

	while (!glfwWindowShouldClose(window)) {
		glfwGetFramebufferSize(window, &render_width, &render_height);

		glViewport(0, 0, render_width, render_height);
		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(static_cast<GLfloat>(0.5 + 0.5*sin(glfwGetTime())), 
			         static_cast<GLfloat>(0.5 + 0.5*sin(glfwGetTime()+2)),
			         static_cast<GLfloat>(0.5 + 0.5*sin(glfwGetTime()+4)), 1.0f);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwDestroyWindow(window);

	glfwTerminate();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}