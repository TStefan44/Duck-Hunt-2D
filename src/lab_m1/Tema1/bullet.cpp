#include "bullet.h"

#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/transform2D.h"

using namespace std;
using namespace m1;

Bullet::Bullet(float posX, float posY) : Objects(posX, posY) {
	length = 0.2;
	glm::vec3 corner = glm::vec3(-length / 2, -length / 2, 0);
	glm::mat3 matrix;

	Mesh* square = object2D::CreateSquare("squareBulletFull", corner, length, glm::vec3(0, 0, 1), true);
	Mesh* square1 = object2D::CreateSquare("squareBulletEmpty", corner, length, glm::vec3(0, 0, 1));

	// Full Bullet
	componentMeshes.push_back(square);
	matrix = glm::mat3(1);
	matrix *= transform2D::Translate(posX, posY);
	matrixComp.push_back(matrix);

	// Empty Bullet
	componentMeshes.push_back(square1);
	matrix = glm::mat3(1);
	matrix *= transform2D::Translate(posX, posY);
	matrixComp.push_back(matrix);
}

Bullet::Bullet() : Bullet(0, 0) {}