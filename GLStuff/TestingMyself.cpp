#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int nopenopenopemain(int argc, char** argv)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* wnd = glfwCreateWindow(800, 600, "Window :)", nullptr, nullptr);

	if (wnd == NULL) {
		std::cout << "Window creation failed. Stopping process." << std::endl;

		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(wnd);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load GL. Stopping process." << std::endl;

		glfwDestroyWindow(wnd);
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);


	// Draw a triangle


	// Triangle Verticies
	const float verts[] {
		-0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f
	};


	uint32_t vertexBufferObject;

	glGenBuffers(1, &vertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	//Bind here
	

	
	// Shader stuff

	const char *const vertexShaderSource = 
		"#version 330 core\n"
		"layout (location=0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
		"}\0";


	const char *const fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 fragColor;"
		"void main()\n"
		"{\n"
		"	fragColor = vec4(0.4f, 0.1f, 0.6f, 1.0f);\n"
		"}\0"
		;


	uint32_t vertexShader, fragmentShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);


	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);


	// compile and check

	int32_t success;

	const int INFOBUFFSIZE = 512;
	char info[INFOBUFFSIZE];

	glCompileShader(vertexShader);


	// Check shader compilation for vertex shader

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, INFOBUFFSIZE, NULL, info);
		std::cout << "Failed to compile vertex shader:\n" << info << std::endl;


	}

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, INFOBUFFSIZE, NULL, info);

		std::cout << "Failed to compile fragment shader:\n" << info << std::endl;

	}



	


	uint32_t shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);


	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, info);
		std::cout << "Failed to link shader program: " << info << "\n";
	}



	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);



	glUseProgram(shaderProgram);



	uint32_t vertexArrayObject;

	glGenVertexArrays(1, &vertexArrayObject);
	
	glBindVertexArray(vertexArrayObject);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
	glEnableVertexAttribArray(0);



	while (!glfwWindowShouldClose(wnd)) {


		
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

		
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwSwapBuffers(wnd);
			glfwPollEvents();


	}
	

	glfwDestroyWindow(wnd);
	glfwTerminate();
	return 0;
	
}