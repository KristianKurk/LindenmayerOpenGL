#pragma once
#include "Branch.h"
#include "Leaf.h"
#include <stack>

using namespace std;

class TurtleState {
public:
	vec3 coords;
	vec3 rotation;
	TurtleState(vec3 coords, vec3 rotation) { this->coords = coords; this->rotation = rotation;}
};

class Turtle {
private:
	vec3 rotation;
	vec3 coords;
	stack<TurtleState> states;
public:
	Turtle();
	Branch CreateBranch(mat4 mvp);
	Leaf CreateLeaf(mat4 mvp);
	void rotate(vec3 rotation);
	void save();
	void restore();
};
