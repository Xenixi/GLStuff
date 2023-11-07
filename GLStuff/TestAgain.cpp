#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

int workingmain(int argc, char **argv) {
	std::cout << "Triangle Renderer Starting..." << std::endl;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);




	GLFWwindow *window = glfwCreateWindow(1600, 900, "Triangle Render :)", nullptr, nullptr);

	if (window == NULL) {
		std::cout << "Failed to create the window." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);




	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load GL." << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 1600, 900);

	const float triangleVerticies[]{
		-0.8f, -0.2f, 0.0f,
		0.8f, -0.05f, 0.0f,
		0.3f, 0.8f, 0.0f
	};



	// load shader - this isn't great but it's temporary for loading the two shaders

	const char *const shadersSrcPaths[2] = { {"vtx.vert"}, {"frg.frag"} };


	std::ifstream fStr(shadersSrcPaths[0]);

	if (!fStr.good()) {
		std::cout << "Failed to locate shader: " << shadersSrcPaths[0] << "." << std::endl;
	}

	std::stringstream buffer;
	buffer << fStr.rdbuf();

	std::string vtxShaderSrcString = buffer.str();
	const char *const vtxShaderSrc = vtxShaderSrcString.c_str();

	fStr.close();

	// *********************

	std::ifstream fStr2(shadersSrcPaths[1]);

	if (!fStr2.good()) {
		std::cout << "Failed to locate shader: " << shadersSrcPaths[1] << "." << std::endl;
	}

	std::stringstream buffer2;
	buffer2 << fStr2.rdbuf();

	std::string frgShaderSrcString = buffer2.str();
	const char *const frgShaderSrc = frgShaderSrcString.c_str();

	fStr2.close();

	// *********************


	/*const char *const vtxShaderSrc =
		"#version 330 core\n"
		"layout (location=0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
		"}\0"
		;
	*/
	/*const char *const frgShaderSrc =
		"#version 330 core\n"
		"out vec4 fragColor;"
		"void main()\n"
		"{\n"
		"	fragColor = vec4(0.1f, 1.0f, 0.2f, 1.0f);\n"
		"}\0"
		;
		*/

	uint32_t vtxShader = glCreateShader(GL_VERTEX_SHADER);
	uint32_t frgShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vtxShader, 1, &vtxShaderSrc, NULL);
	glShaderSource(frgShader, 1, &frgShaderSrc, NULL);

	glCompileShader(vtxShader);
	glCompileShader(frgShader);

	int success;
	const int infoSize = 512;
	char infoLog[infoSize];

	glGetShaderiv(vtxShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vtxShader, infoSize, NULL, infoLog);
		std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}

	glGetShaderiv(frgShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(frgShader, infoSize, NULL, infoLog);
		std::cout << "Fragment shader compilation failed." << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}

	uint32_t shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vtxShader);
	glAttachShader(shaderProgram, frgShader);

	glLinkProgram(shaderProgram);

	// Buffers and stuff

	uint32_t vertexBufferObject;

	glGenBuffers(1, &vertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVerticies), &triangleVerticies, GL_STATIC_DRAW);

	uint32_t vertexArrayObject;

	glGenVertexArrays(1, &vertexArrayObject);

	glBindVertexArray(vertexArrayObject);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);



	// Notes on mistakes:
	/*
	* Didn't bind the buffer or array
	* Didn't enable the VertexAttribArray at index
	* Didn't pass in (void*)0 to last arg of glVertexAttribPointer()
	*/

	glUseProgram(shaderProgram);

	glfwSwapInterval(1); // sync

	double time = glfwGetTime();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


	while (!glfwWindowShouldClose(window)) {
		printf("%ffFPS\n", 1 / (glfwGetTime() - time));
		time = glfwGetTime();
		
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

	


		glfwSwapBuffers(window);

		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();
	return -1;


}