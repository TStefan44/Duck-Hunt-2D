#pragma once

#include "objects.h"

namespace m1
{
	class Heart : public Objects
	{
	public:
		Heart();
		Heart(float posX, float posY);

	protected:
		float radius;
	};
}