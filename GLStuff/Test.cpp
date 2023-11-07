#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>

int nopemain(int argc, char **argv) {

	std::cout << "Starting!!\n";

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *wnd = glfwCreateWindow(900, 900, "Hey :)", nullptr, nullptr);

	if (wnd == NULL) {
		std::cout << "Failed to create window. Shutting down.\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(wnd);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, 900, 900);


	// Tri verts

	float verts[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f, 0.5f, 0.0f
	};

	// *******************


	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	const char *const shader =
		"#version 330 core\n"
		"layout (location=0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
		"}\0";


	




	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &shader, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {

		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "shader compilation failed: " << infoLog << "\n";

	}

	unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char *const fragSource =
		"#version 330 core\n"
		"out vec4 fragColor;"
		"void main()\n"
		"{\n"
		"	fragColor = vec4(0.4f, 0.1f, 0.6f, 1.0f);"
		"}"
		;

	glShaderSource(fragShader, 1, &fragSource, NULL);

	glCompileShader(fragShader);

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Shader compilation failed : frag shader : " << infoLog << "\n";
	}


	unsigned int shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	// Shaders not needed after linking

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	//


	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Failed to link shader program: " << infoLog << "\n";
	}



	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// We already did this before;
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);




	glUseProgram(shaderProgram);

	// VAO 


	unsigned int VAO;

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);

	glEnableVertexAttribArray(0);



	//glUseProgram(shaderProgram);

	glBindVertexArray(VAO);




	while (!glfwWindowShouldClose(wnd)) {





		glfwPollEvents();



		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(wnd);



	}

	glfwTerminate();

	return 0;

}