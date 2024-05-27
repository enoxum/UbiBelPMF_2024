#pragma once

#include "core/system.h"
#include "core/core.h"
#include "deadend_health.h"

using namespace dagger;

namespace dead_end
{
	struct Player
	{
		Float32 speed {300.f};
		int weaponType{ 1 }; // 1 - pistol, 2 - gun, 3-shotgun
		bool stopMoving{ false };
		int killCount{ 0 };
		std::pair<bool, Float32> hit{ false, 0.f };
	};

	class PlayerSystem
		: public System
	{
	public:

		inline String SystemName() { return "Player System"; }

		void Run() override;

		void handleHit(Player& player_, Health& health_);
	};

	
}
