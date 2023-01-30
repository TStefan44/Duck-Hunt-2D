#pragma once

#include "objects.h"

namespace m1
{
	class Aim : public Objects
	{
	public:
		Aim();
		Aim(float posX, float posY);

		void setPosition(float posX, float posY, float deltaTimeSeconds = 1) override;

	protected:
		float length;
		float rotAngleS1, angularSpeedS1;

		void updatePosition() override;
	};
}