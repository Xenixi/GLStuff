#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(int argc, char **argv) {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *wnd = glfwCreateWindow(400, 400, "Hey :)", nullptr, nullptr);

	if (wnd == NULL) {
		std::cout << "Failed to create window. Process terminating..." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(wnd);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load GL. Process terminating..." << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0,0,400,400);

	// Shaders

	const char *const vShdSrc =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos\n;"
		"out vec4 colorFinal;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"colorFinal = vec4(abs(aPos.x), abs(aPos.y), 0.5, 1.0);\n"
		"}\0"
		;

	const char *const fShdSrc =
		"#version 330 core\n"
		"in vec4 colorFinal;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = colorFinal;\n"
		"}\0"
		;

	uint32_t vShd, fShd;

	vShd = glCreateShader(GL_VERTEX_SHADER);
	fShd = glCreateShader(GL_FRAGMENT_SHADER);
	

	glShaderSource(vShd, 1, &vShdSrc, NULL);
	glCompileShader(vShd);
	glShaderSource(fShd, 1, &fShdSrc, NULL);
	glCompileShader(fShd);
	

	int success; 
	char info[512];

	glGetShaderiv(vShd, GL_COMPILE_STATUS, &success);
	
	if (!success) {
		glGetShaderInfoLog(vShd, 512, NULL, info);
		std::cout << "Failed to compile vertex shader:\n" << info << std::endl;
	}

	glGetShaderiv(fShd, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fShd, 512, NULL, info);
		std::cout << "Failed to compile fragment shader:\n" << info << std::endl;
	}

	uint32_t shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vShd);
	glAttachShader(shaderProgram, fShd);

	glLinkProgram(shaderProgram);

	glDeleteShader(vShd);
	glDeleteShader(fShd);

	//

	const float triVerts[]{
		
		0.0f, 0.75f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.75f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f,

	};

	uint32_t vtxBufferObject;
	glGenBuffers(1, &vtxBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vtxBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triVerts), triVerts, GL_STATIC_DRAW);

	uint32_t vtxArrayObject; 
	glGenVertexArrays(1, &vtxArrayObject);
	glBindVertexArray(vtxArrayObject);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glUseProgram(shaderProgram);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(wnd)) {
		
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 1, 3);
		
	

		glfwSwapBuffers(wnd);
		glfwPollEvents();
	}

	glfwDestroyWindow(wnd);
	glfwTerminate();
	return 0;
}