#pragma once

#include "objects.h"

namespace m1
{
	class Background : public Objects
	{
	public:
		Background();
		Background(float posX, float posY);

		void setPosition(float X, float Y, float deltaTimeSeconds = 1) override;

	protected:
		float length;
		float scale;
		float angularRot;
		float angularSpeed;

		void updatePosition() override;
	};
}