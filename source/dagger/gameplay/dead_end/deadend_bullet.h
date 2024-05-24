#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace dead_end
{

	struct Bullet {
		int damage{ 10 };
		Float32 speed = 200.f;
		Float32 speedX = 100.f;
		Float32 speedY = 100.f;
		Vector2 direction;
		float playerOffset{5.0f};

	};

	class ShootingSystem
		: public System
	{
		inline String SystemName() { return "Shooting System"; }

		void Run() override;
	};

	void CreateBullet(Vector2 position_, Vector2 target_, int weaponType_);
}