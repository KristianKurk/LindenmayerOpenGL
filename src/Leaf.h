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

class Leaf {
public:
	void DrawTriangle();
	Leaf(vec3 origin, float length, float width, vec3 rot, vec3 color);
	Leaf(vec3 P1, vec3 P2, vec3 P3, vec3 color);
	void SetColor(vec3 color);
	int SetMVP(mat4 mvp);
	void Animate(float deltaTime);
	~Leaf();
private:
	void Initialise(vec3 P1, vec3 P2, vec3 P3, vec3 color);
	int shaderProgram;
	unsigned int VBO, VAO, EBO;
	vector<float> vertices;
	mat4 MVP;
	vec3 P1;
	vec3 P2;
	vec3 P3;
	vec3 color;
	const char* GetFragmentShader();
	const char* GetVertexShader();
};