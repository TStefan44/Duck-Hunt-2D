#pragma once

#include <vector>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace m1
{
	class Objects
	{
	public:
		Objects();
		Objects(float posX, float posY);
		Objects(float posX, float posY, float weightBoundigBox, float  heightBoundingBox);

		virtual void setPosition(float X, float Y, float deltaTimeSeconds = 1);
		std::vector<Mesh*> getComponentMeshes();
		std::vector<glm::mat3> getMatrixComp();
		glm::vec4 getBoundingBox();
		bool BoundingBoxCollision(Objects obj);

	protected:
		float posX, posY;
		float weightBoundigBox, heightBoundingBox;
		std::vector<Mesh*> componentMeshes;
		std::vector<glm::mat3> matrixComp;

		virtual void updatePosition();
	};
}