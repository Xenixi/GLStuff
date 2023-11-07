#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <limits>

int main(int argc, char **argv) {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *wnd = glfwCreateWindow(1280, 720 , "Hey :)", nullptr, nullptr);

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

	glViewport(0,0,1280,720);

	// Shaders

	const char *const shadersSrcPaths[2] = { {"vtx.vert"}, {"frg.frag"} };


	std::ifstream fStr(shadersSrcPaths[0]);

	if (!fStr.good()) {
		std::cout << "Failed to locate shader: " << shadersSrcPaths[0] << "." << std::endl;
	}

	std::stringstream buffer;
	buffer << fStr.rdbuf();

	std::string vtxShaderSrcString = buffer.str();
	const char *const vShdSrc = vtxShaderSrcString.c_str();

	fStr.close();

	// *********************

	std::ifstream fStr2(shadersSrcPaths[1]);

	if (!fStr2.good()) {
		std::cout << "Failed to locate shader: " << shadersSrcPaths[1] << "." << std::endl;
	}

	std::stringstream buffer2;
	buffer2 << fStr2.rdbuf();

	std::string frgShaderSrcString = buffer2.str();
	const char *const fShdSrc = frgShaderSrcString.c_str();


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

	uint32_t uniformAssignedLoc = glGetUniformLocation(shaderProgram, "adjusted");

	uint8_t adjusted = 0;
	
	double_t time = glfwGetTime();

	while (!glfwWindowShouldClose(wnd)) {
	

		if (glfwGetTime() - time >= 0.02f) {
			adjusted += 5;
			time = glfwGetTime();
		}

		glUniform1f(uniformAssignedLoc, fabsf(2.0f * ((float_t)adjusted / std::numeric_limits<uint8_t>::max()) - 1.0f));
		
		

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