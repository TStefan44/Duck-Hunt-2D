#pragma once

#include "objects.h"

namespace m1
{
	class Duck : public Objects
	{
	public:
		Duck();
		Duck(float posX, float posY);
		Duck(float posX, float posY, float currentSpeed);

		enum class State { ACTIVE, FLEE, DEAD, RESET};

		void activeState(float logicY, float deltaTimeSeconds = 1);
		void fleeState(float logicY, float deltaTimeSeconds = 1);
		void deadState(float logicY, float deltaTimeSeconds = 1);
		void resetState(float posX, float posY, float currentSpeed, float angularSpeed);

		void setPosition(float logicX, float logicY, float deltaTimeSeconds = 1) override;
		void setStartPosition(float posX, float posY);
		void setSpeed(float currentSpeed);
		void setState(State currentState);
		State getState();
		float getInitialAngularSpeed();

		void generateNewDirection();

	protected:
		float maxSpeed;
		float currentSpeed;
		float dirAngle;
		float right, up;

		float initialAngularSpeed;
		float angularSpeed;
		float wingRot;
		bool trigRot;

		float duckAngle;
		glm::mat3 mirrorOx, mirrorOy;

		State currentState;

		void updatePosition() override;
	};
}