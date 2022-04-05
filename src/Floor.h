#pragma once
#define GLEW_STATIC 1

#include <A:/Comp371/libraries/glew-2.1.0/include/GL/glew.h>
#include <A:/Comp371/libraries/glfw-3.3.6/include/GLFW/glfw3.h>
#include <A:/Comp371/libraries/glm/glm/glm.hpp>
#include "A:/Comp371/libraries/glm/glm/gtc/matrix_transform.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace glm;

class Floor {
public:
	void DrawTriangle();
	Floor(vec3 P1, vec3 P2, vec3 P3, vec3 P4, vec3 color);
	void SetColor(vec3 color);
	int SetMVP(mat4 mvp);
	~Floor();
	vec3 P1;
	vec3 P2;
	vec3 P3;
	vec3 P4;
private:
	void Initialise(vec3 P1, vec3 P2, vec3 P3, vec3 P4, vec3 color);
	int shaderProgram;
	unsigned int VBO, VAO, EBO;
	vector<float> vertices;
	mat4 MVP;
	vec3 color;
	const char* GetFragmentShader();
	const char* GetVertexShader();
};