#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace dead_end
{

	struct DeadEndCamera
	{
		int weight{ 1 };
		Vector3 position{ 0,0,0 };
		Vector2 size{ 800, 600 };
	};

	class DeadEndCameraFollowSystem
		: public System
	{
		inline String SystemName() override { return "DeadEnd Camera System."; }

		void Run() override;
	};
}