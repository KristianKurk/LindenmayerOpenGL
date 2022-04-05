#include <iostream>

#define GLEW_STATIC 1

#include <A:/Comp371/libraries/glew-2.1.0/include/GL/glew.h>
#include <A:/Comp371/libraries/glfw-3.3.6/include/GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <A:/Comp371/libraries/glm/glm/glm.hpp>
#include "A:/Comp371/libraries/glm/glm/gtc/matrix_transform.hpp"
#include <A:/Comp371/libraries/glm/glm/gtc/type_ptr.hpp>
#include "src/Turtle.h"
#include <vector>
#include "src/Floor.h"

using namespace std;

const GLint WIDTH = 800;
const GLint HEIGHT = 600;
string lsystem;
vector<Branch> branches;
vector<Leaf> leafs;
mat4 mvp;
int iterations = 1;
string axiom = "X";
float leafChance = 0.5f;
int degAngle = 30;

string ApplyRuleToSentence(string sentence) {
	string newSentence;

	for (unsigned int i = 0; i < sentence.length(); i++) {
		char c = sentence.at(i);

		if (c == 'X') {
			int choice = rand() % 8;
			switch (choice) {
			case 0:
				newSentence += "F/-[[X]+X]+F[[X]+X]-X";
				break;
			case 1:
				newSentence += "F*-[[X]+X]+F[-FX]*+X";
				break;
			case 2:
				newSentence += "F[/+X][-X]FX";
				break;
			case 3:
				newSentence += "F[*+X]F[-X]/+X";
				break;
			case 4:
				newSentence += "F-/[[X]-*X]+*F[[X]*X]/X";
				break;
			case 5:
				newSentence += "F/[[X]+*X]-*F[/FX]-*X";
				break;
			case 6:
				newSentence += "F[*X][+/X]FX";
				break;
			case 7:
				newSentence += "F[*X]F[/X]/*X";
				break;
			}
		}
		else if (c == 'F') {
			newSentence += "FF";
		}
		else {
			newSentence += c;
		}
	}
	return newSentence;
}

string ApplySystemRules(string axiom, int iterations) {
	string sentence = axiom;

	for (unsigned int i = 0; i < iterations; i++) {
		sentence = ApplyRuleToSentence(sentence);
	}
	return sentence;
}

void CreateSystem(string lsystem, mat4 mvp, vector<Branch>& branches, vector<Leaf>& leafs) {
	Turtle turtle;

	float angle = (3.14f / 180) * degAngle;

	for (int i = 0; i < lsystem.length(); i++) {
		char c = lsystem[i];

		if (c == 'F') {
			branches.push_back(turtle.CreateBranch(mvp));
		}
		else if (c == 'X') {
			if (i + 1 < lsystem.size()) {
				if (lsystem.at(i + 1) != '[') {
					float luck = ((float)rand() / RAND_MAX);

					if (luck < leafChance)
						leafs.push_back(turtle.CreateLeaf(mvp));
				}
			}
		}
		else if (c == '+') {
			turtle.rotate(vec3(angle, 0, 0));
		}
		else if (c == '-') {
			turtle.rotate(-vec3(angle, 0, 0));
		}
		else if (c == '*') {
			turtle.rotate(vec3(0, angle, 0));
		}
		else if (c == '/') {
			turtle.rotate(-vec3(0, angle, 0));
		}
		else if (c == '[') {
			turtle.save();
		}
		else if (c == ']') {
			turtle.restore();
		}
	}
}

void DrawSystem(vector<Branch> branches, vector<Leaf> leafs, mat4 mvp) {
	for (int i = 0; i < branches.size(); i++) {
		branches[i].SetMVP(mvp);
		branches[i].DrawLine();
	}

	for (int i = 0; i < leafs.size(); i++) {
		leafs[i].SetMVP(mvp);
		leafs[i].DrawTriangle();
	}
}

void DrawFloor(Floor floor, vector<Leaf> grass, mat4 mvp) {
	floor.SetMVP(mvp);
	floor.DrawTriangle();

	for (int i = 0; i < grass.size(); i++) {
		grass[i].SetMVP(mvp);
		grass[i].DrawTriangle();
	}
}

Floor CreateFloor(vector<Leaf>& grass) {



	Floor floor = Floor(vec3(1, 0, -1), vec3(-1, 0, 1), vec3(-1, 0, -1), vec3(1, 0, 1), vec3(0, 0, 0));
	int grassDensity = 10;
	vec3 diff = floor.P4 - floor.P3;
	int random = rand() % 3;

	switch (random) {
	case 0:
		floor.SetColor(vec3(0.16f, 0.21f, 0.16f));
		break;
	case 1:
		floor.SetColor(vec3(0.28f, 0.40f, 0.28f));
		break;
	case 2:
		floor.SetColor(vec3(0.18f, 0.5f, 0.28f));
		break;
	}


	for (int i = 0; i < grassDensity; i++) {
		for (int j = 0; j < grassDensity; j++) {
			vec3 pos = vec3(((float)rand() / (RAND_MAX)) * 2 - 1, 0, ((float)rand() / (RAND_MAX)) * 2 - 1);
			Leaf grassBlade = Leaf(pos, 0.5f, 0.1f, vec3(0, 0, 0), vec3(0.16f, 0.21f, 0.16f));

			int random = rand() % 3;

			switch (random) {
			case 0:
				grassBlade.SetColor(vec3(0.16f, 0.21f, 0.16f));
				break;
			case 1:
				grassBlade.SetColor(vec3(0.28f, 0.40f, 0.28f));
				break;
			case 2:
				grassBlade.SetColor(vec3(0.18f, 0.5f, 0.28f));
				break;
			}
			grass.push_back(grassBlade);
		}
	}

	return floor;
}

mat4 GetCameraView(float cameraDistance, float moveDelta, float rotationSpeed) {
	float angle = moveDelta * rotationSpeed;

	vec3 cameraPos = cameraDistance * vec3(cos(radians(angle)), 1, sin(radians(angle)));
	mat4 view = lookAt(cameraPos, vec3(0, 2, 0), vec3(0, 1, 0));
	return view;
}

void GetInput(GLFWwindow* window, float& moveDelta, float& cameraDistance) {
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		moveDelta += 0.02f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		moveDelta -= 0.02f;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && cameraDistance > 1) {
		cameraDistance -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cameraDistance += 0.1f;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		srand(time(0));
		branches.clear();
		leafs.clear();
		lsystem = ApplySystemRules(axiom, iterations);
		CreateSystem(lsystem, mvp, branches, leafs);
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		iterations++;
		branches.clear();
		leafs.clear();
		lsystem = ApplySystemRules(axiom, iterations);
		CreateSystem(lsystem, mvp, branches, leafs);
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS && iterations > 1)
	{
		iterations--;
		branches.clear();
		leafs.clear();
		lsystem = ApplySystemRules(axiom, iterations);
		CreateSystem(lsystem, mvp, branches, leafs);
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS && iterations > 1)
	{
		leafChance += 0.25f;
		if (leafChance > 1.0f) leafChance = 0.0f;

		branches.clear();
		leafs.clear();
		CreateSystem(lsystem, mvp, branches, leafs);
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && degAngle > 0) {
		degAngle -= 15;
		branches.clear();
		leafs.clear();
		CreateSystem(lsystem, mvp, branches, leafs);
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && degAngle < 90) {
		degAngle += 15;
		branches.clear();
		leafs.clear();
		CreateSystem(lsystem, mvp, branches, leafs);
	}
}

int main(int argc, char* argv[])
{
	// Initialize GLFW and OpenGL version
	if (!glfwInit()) {
		cerr << "Failed to initialise glfw";
	};

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Final Project - Kristian Kurktchiev", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

	//Set GLFW context
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	//Setup viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	glfwSetKeyCallback(window, key_callback);

	//Generate System
	axiom = "X";
	lsystem = ApplySystemRules(axiom, iterations);

	//Camera
	float cameraDistance = 5;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float rotationSpeed = 50.0f;
	float moveDelta = 0.0f;

	mat4 projection = perspective(radians(45.0f), (float)bufferWidth / (float)bufferHeight, 0.1f, 100.0f);

	glEnable(GL_LINE_SMOOTH);

	vector<Leaf> grass;
	Floor floor = CreateFloor(grass);

	CreateSystem(lsystem, projection, branches, leafs);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;

		//Clear window
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		mvp = projection * GetCameraView(cameraDistance, moveDelta, rotationSpeed);

		for (int i = 0; i < leafs.size(); i++)
			leafs[i].Animate(deltaTime);

		for (int i = 0; i < grass.size(); i++)
			grass[i].Animate(deltaTime);

		DrawSystem(branches, leafs, mvp);
		DrawFloor(floor, grass, mvp);

		//Handle user input and display window
		GetInput(window, moveDelta, cameraDistance);
		glfwPollEvents();
		glfwSwapBuffers(window);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}
	glDisable(GL_LINE_SMOOTH);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}