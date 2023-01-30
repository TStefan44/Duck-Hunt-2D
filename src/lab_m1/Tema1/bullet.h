#pragma once

#include "objects.h"

namespace m1
{
	class Bullet : public Objects
	{
	public:
		Bullet();
		Bullet(float posX, float posY);

	protected:
		float length;
	};
}