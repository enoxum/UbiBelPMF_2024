#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace dead_end
{
	struct DeadEndObstacle {
		bool isObstacle{ true };
	};

	class DeadEndObstacleSystem
		: public System
	{

	public:

		inline String SystemName() { return "DeadEnd Obstacle System"; }
	};
}
