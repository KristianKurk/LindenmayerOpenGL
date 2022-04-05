#pragma once
#define GLEW_STATIC 1

#define GLEW_STATIC 1

#include <A:/Comp371/libraries/glew-2.1.0/include/GL/glew.h>
#include <A:/Comp371/libraries/glfw-3.3.6/include/GLFW/glfw3.h>
#include <A:/Comp371/libraries/glm/glm/glm.hpp>
#include "A:/Comp371/libraries/glm/glm/gtc/matrix_transform.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace glm;

class Branch {
public:
	vec3 DrawLine();
	Branch(vec3 origin, float length, vec3 rotation, vec3 color);
	Branch(vec3 origin, vec3 endPoint, vec3 color);
	int SetMVP(mat4 mvp);
	void SetColor(vec3 newColor);
	~Branch();
private:
	void Init(vec3 origin, vec3 endPoint, vec3 color);
	int shaderProgram;
	unsigned int VBO, VAO, EBO;
	vector<float> vertices;
	mat4 MVP;
	vec3 startPoint;
	vec3 endPoint;
	vec3 color;
	const char* GetFragmentShader();
	const char* GetVertexShader();
	void CompileShaders();
};