#include "background.h"

#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/transform2D.h"

using namespace std;
using namespace m1;

Background::Background(float posX, float posY) : Objects(posX, posY) {
	length = 1.0f;
	angularRot = 0;
	angularSpeed = M_PI / 10;
	glm::mat3 matrix;

	std::vector<glm::vec3> color =
	{
		glm::vec3(237.f / 255.f, 135.f / 255.f, 45.f / 255.f),
		glm::vec3(237.f / 255.f, 135.f / 255.f, 45.f / 255.f),
		glm::vec3(81.f / 255.f, 40.f / 255.f, 137.f / 255.f),
		glm::vec3(81.f / 255.f, 40.f / 255.f, 137.f / 255.f)

	};
	Mesh* bgSquare1 = object2D::CreateSquare("bgSquare1", glm::vec3(-length / 2.f, -length / 2.f, 0), length, color, true);
	Mesh* bgSquare2 = object2D::CreateSquare("bgSquare2", glm::vec3(-length / 2.f, -length / 2.f, 0), length, color, true);


	componentMeshes.push_back(bgSquare1);
	componentMeshes.push_back(bgSquare2);
	matrix = glm::mat3(1);
	matrix *= transform2D::Translate(posX, posY);
	matrixComp.push_back(matrix);
	matrixComp.push_back(matrix);
}

Background::Background() : Background(0, 0) {}

void Background::setPosition(float X, float Y, float deltaTimeSeconds) {
	posX = X;
	posY = Y;
	scale = max(X * 2, Y * 2);
	angularRot += angularSpeed * deltaTimeSeconds;

	updatePosition();
}

void Background::updatePosition() {
	matrixComp[0] = glm::mat3(1) *
		transform2D::Translate(posX, posY) * transform2D::Scale(2* scale, 2 * scale) *
		transform2D::Rotate(angularRot);
}