#include "objects.h"

using namespace std;
using namespace m1;

Objects::Objects(float posX, float posY, float weightBoundigBox, float  heightBoundingBox)
	: posX(posX), posY(posY), weightBoundigBox(weightBoundigBox), heightBoundingBox(heightBoundingBox) {}

Objects::Objects(float posX, float posY) : Objects(posX, posY, 1, 1) {}

Objects::Objects() : Objects(0, 0) {};

void Objects::setPosition(float X, float Y, float deltaTimeSeconds) {
	posX = X;
	posY = Y;

	updatePosition();
}

void Objects::updatePosition() {}

std::vector<Mesh*> Objects::getComponentMeshes() {
	return componentMeshes;
}

std::vector<glm::mat3> Objects::getMatrixComp() {
	return matrixComp;
}

bool Objects::BoundingBoxCollision(Objects obj) {
	glm::vec4 dim1 = getBoundingBox();
	glm::vec4 dim2 = obj.getBoundingBox();

	if (
		dim1.x < dim2.x + dim2.z &&
		dim1.x + dim1.z > dim2.x &&
		dim1.y < dim2.y + dim2.w &&
		dim1.w + dim1.y > dim2.y
		) {
		return true;
	}
	else {
		return false;
	}
}

glm::vec4 Objects::getBoundingBox() {
	float weight = weightBoundigBox;
	float height = heightBoundingBox;
	return glm::vec4(posX - weight / 2, posY - height / 2, weight, height);
}