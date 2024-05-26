#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace dead_end
{
	struct Player
	{
		Float32 speed {300.f};
		int weaponType{ 1 }; // 1 - pistol, 2 - gun
		bool stopMoving = false;
		int killCount{ 0 };
	};

	class PlayerSystem
		: public System
	{
	public:

		inline String SystemName() { return "Player System"; }

		void Run() override;
	};
}
