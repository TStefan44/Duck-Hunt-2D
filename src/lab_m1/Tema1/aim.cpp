#include "aim.h"

#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/transform2D.h"

using namespace std;
using namespace m1;


Aim::Aim(float posX, float posY) : Objects(posX, posY, 1, 1) {
	length = 1;
	angularSpeedS1 = M_PI_4;
	rotAngleS1 = 0;
	glm::vec3 corner = glm::vec3(- length / 2, - length / 2, 0);
	glm::vec3 center = glm::vec3(0, 0, 0);
	glm::mat3 matrix;

	Mesh* square1 = object2D::CreateSquare("squareAim1", corner, length, glm::vec3(1, 0, 0));
	Mesh* square2 = object2D::CreateSquare("squareAim2", corner, length, glm::vec3(1, 0, 0));
	Mesh* circle = object2D::CreatePolygon("circleAim", center, length * 0.1, 50, glm::vec3(1, 0, 0), true);

	// Square 1
	componentMeshes.push_back(square1);
	matrix = glm::mat3(1);
	matrix *= transform2D::Translate(posX, posY);
	matrixComp.push_back(matrix);

	// Square 2
	componentMeshes.push_back(square2);
	matrix = glm::mat3(1);
	matrix *= transform2D::Translate(posX, posY) *
			  transform2D::Rotate(M_PI_4);
	matrixComp.push_back(matrix);

	// Circle
	componentMeshes.push_back(circle);
	matrix = glm::mat3(1);
	matrix *= transform2D::Translate(posX, posY);
	matrixComp.push_back(matrix);

}

Aim::Aim() : Aim(0, 0) {};

void Aim::setPosition(float posX, float posY, float deltaTimeSeconds)
{
	this->posX = posX;
	this->posY = posY;

	rotAngleS1 += angularSpeedS1 * deltaTimeSeconds;

	updatePosition();
}

void Aim::updatePosition() {
	matrixComp[0] = glm::mat3(1) *
					transform2D::Translate(posX, posY) *
					transform2D::Rotate(M_PI_4 - rotAngleS1);
	matrixComp[1] = glm::mat3(1) *
					transform2D::Translate(posX, posY) *
					transform2D::Rotate(M_PI_4 + rotAngleS1);
	matrixComp[2] = glm::mat3(1) *
					transform2D::Translate(posX, posY);

}