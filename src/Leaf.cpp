#include "Leaf.h"

void Leaf::DrawTriangle()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(shaderProgram);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &color[0]);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

int Leaf::SetMVP(mat4 mvp) {
	this->MVP = mvp;
	return 1;
}

Leaf::Leaf(vec3 origin, float length, float width, vec3 rot, vec3 color)
{
	vec3 direction = length * vec3(-cos(rot.z) * sin(rot.x) * sin(rot.y) - sin(rot.z) * cos(rot.y), -sin(rot.z) * sin(rot.x) * sin(rot.y) + cos(rot.z) * cos(rot.x), cos(rot.x) * sin(rot.y));
	vec3 P1 = origin - width / 2;
	vec3 P2 = origin + width / 2;
	vec3 P3 = origin + direction;
	Initialise(P1, P2, P3, color);
}

Leaf::Leaf(vec3 P1, vec3 P2, vec3 P3, vec3 color)
{
	Initialise(P1, P2, P3, color);
}

void Leaf::SetColor(vec3 color)
{
	this->color = color;
}

const char* Leaf::GetFragmentShader()
{
	return
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec3 color;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(color, 1.0f);\n"
		"}\n\0";
}

const char* Leaf::GetVertexShader()
{
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 MVP;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
}

Leaf::~Leaf() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
}

void Leaf::Animate(float deltaTime) {
	vec3 windDirection = vec3(0, 0, 1);
	float animSpeed = 2;

	vec3 diff = P2 - P1;
	float magnitude = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
	vec3 newP3 = P3 + (sin(deltaTime * animSpeed) * (windDirection * (magnitude)));
	this->vertices = {
		P1.x, P1.y, P1.z,
		P2.x, P2.y, P2.z,
		newP3.x, newP3.y, newP3.z
	};
}

void Leaf::Initialise(vec3 P1, vec3 P2, vec3 P3, vec3 color)
{
	this->P1 = P1;
	this->P2 = P2;
	this->P3 = P3;
	this->color = color;
	this->MVP = mat4(1.0);

	const char* vertexShaderSource = GetVertexShader();
	const char* fragmentShaderSource = GetFragmentShader();

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	this->vertices = {
	 P1.x, P1.y, P1.z,
	 P2.x, P2.y, P2.z,
	 P3.x, P3.y, P3.z
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}