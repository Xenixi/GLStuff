#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <limits>
#include "stb_image.h"

int main(int argc, char **argv) {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *wnd = glfwCreateWindow(720, 720 , "Hey :)", nullptr, nullptr);

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

	glViewport(0,0,720,720);

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
		
		-0.5f, 0.5f, 0.0f,    -1.0f, 1.0f, 1.0f,  //get rid of the z 
		0.5f, 0.5f, 0.0f,     0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,   -1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,


	};
	
	const uint32_t triIndicies[]{
		0, 1, 2,
		1, 2, 3
	};

	uint32_t vtxBufferObject;
	glGenBuffers(1, &vtxBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vtxBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triVerts), triVerts, GL_STATIC_DRAW);


	uint32_t vtxArrayObject; 
	glGenVertexArrays(1, &vtxArrayObject);
	glBindVertexArray(vtxArrayObject);


	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glUseProgram(shaderProgram);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //wireframe


	// Allows reuse of vertecies by specifying an array of indicies. 
	uint32_t elementBufferObject;
	glGenBuffers(1, &elementBufferObject);

	//must be bound after VAO is bound. For VBO, binding to VAO happens with glVertexAttribPointer()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject); 
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triIndicies), triIndicies, GL_STATIC_DRAW);

	// TEXTURE

	

	int32_t tWidth, tHeight, tChannels;

	unsigned char *imageData = stbi_load("c:/users/kobes/onedrive/desktop/texture.jpg", &tWidth, &tHeight, &tChannels, 0);

	if (imageData == NULL) {
		std::cout << "ERROR LOADING TEXTURE" << std::endl;
	}

	uint32_t texture;

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	stbi_image_free(imageData);

	////

	uint8_t adjusted = 0;
	
	double_t time = glfwGetTime();

	glBindTexture(GL_TEXTURE_2D, texture);

	while (!glfwWindowShouldClose(wnd)) {
	

		

		glClear(GL_COLOR_BUFFER_BIT);

//		glDrawArrays(GL_TRIANGLES, 0, 3);
//		glDrawArrays(GL_TRIANGLES, 1, 3);


		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	

		glfwSwapBuffers(wnd);
		glfwPollEvents();
	}

	glfwDestroyWindow(wnd);
	glfwTerminate();
	return 0;
}