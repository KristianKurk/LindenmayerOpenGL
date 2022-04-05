#include "Turtle.h"

Turtle::Turtle()
{
	this->rotation = vec3(0);
	this->coords = vec3(0);
}

Branch Turtle::CreateBranch(mat4 mvp)
{
	Branch line = Branch(this->coords, .1f, this->rotation, vec3(0.4, 0.2, 0));
	line.SetMVP(mvp);
	this->coords = line.DrawLine();
	return line;
}

Leaf Turtle::CreateLeaf(mat4 mvp)
{
	Leaf triangle = Leaf(this->coords, .1f, .1f, this->rotation, vec3(0, 0, 0));
	int random = rand() % 3;

	switch (random) {
		case 0:
			triangle.SetColor(vec3(0.16f, 0.21f, 0.16f));
			break;
		case 1:
			triangle.SetColor(vec3(0.28f, 0.40f, 0.28f));
			break;
		case 2:
			triangle.SetColor(vec3(0.18f, 0.5f, 0.28f));
			break;
	}
	triangle.SetMVP(mvp);
	triangle.DrawTriangle();
	return triangle;
}

void Turtle::rotate(vec3 rotation)
{
	this->rotation += rotation;
}

void Turtle::save()
{
	states.push(TurtleState(this->coords, this->rotation));
}

void Turtle::restore()
{
	if (states.empty()) return;

	TurtleState state = states.top();
	states.pop();
	this->coords = state.coords;
	this->rotation = state.rotation;
}
