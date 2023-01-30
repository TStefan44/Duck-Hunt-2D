#include "heart.h"

#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/transform2D.h"

using namespace std;
using namespace m1;

Heart::Heart(float posX, float posY) : Objects(posX, posY) {
	radius = 0.12;
	glm::vec3 center = glm::vec3(0, 0, 0);
	glm::mat3 matrix;

	Mesh* circle = object2D::CreatePolygon("heartFull", center, radius, 50, glm::vec3(0, 0, 1), true);
	Mesh* circle1 = object2D::CreatePolygon("heartEmpty", center, radius, 50, glm::vec3(0, 0, 1));

	// Full Heart
	componentMeshes.push_back(circle);
	matrix = glm::mat3(1);
	matrix *= transform2D::Translate(posX, posY);
	matrixComp.push_back(matrix);

	// Empty Heart
	componentMeshes.push_back(circle1);
	matrix = glm::mat3(1);
	matrix *= transform2D::Translate(posX, posY);
	matrixComp.push_back(matrix);
}

Heart::Heart() : Heart(0, 0) {}