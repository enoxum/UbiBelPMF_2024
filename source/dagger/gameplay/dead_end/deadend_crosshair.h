#pragma once
#include "core/system.h"
#include "core/engine.h"
#include "core/core.h"

using namespace dagger;

namespace dead_end
{
	struct Aim
	{
		Entity crosshairSprite;
		Float32 playerDistance{ 25.0f };
	};

	class AimSystem
		: public System
	{
		inline String SystemName() { return "DeadEnd Aim System"; }

		void Run() override;


	};
}