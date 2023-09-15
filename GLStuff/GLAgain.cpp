#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <time.h>

int main(int argc, char** argv) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(400, 400, "Colors", NULL, NULL);

	if (window == NULL) {

		std::cout << "failed to create window." << std::endl;

		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);



	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

		std::cout << "GLAD failed to initialize." << std::endl;

		glfwTerminate();
		return -1;

	}
	glViewport(0, 0, 400, 400);



	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h) {
		glViewport(0, 0, w, h);
		});
	float a = 0.0f, b = 1.0f;

	auto time = std::chrono::steady_clock::now();

	

	while (!glfwWindowShouldClose(window)) {

		glClearColor(a, b, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time).count() >= 1000) {
			time = std::chrono::steady_clock::now();
			a = !a; //jank af
			b = !b; //

		}


	}

	glfwTerminate();

	return 0;


	//decent
}