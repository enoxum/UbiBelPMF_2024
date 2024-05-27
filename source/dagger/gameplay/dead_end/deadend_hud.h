#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace dead_end
{
	struct HUD 
	{
		Vector2 position {300.f, 250.f};
		Vector2 size{ 130.f, 130.f };
	};

	class DeadEndHUDSystem
		: public System
	{
		inline String SystemName() override { return "HUD system"; }

		void Run() override;
	};
}
