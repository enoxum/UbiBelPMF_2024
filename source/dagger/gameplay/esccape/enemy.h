#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace esccape
{
	struct EnemyEntity
	{
		int health;
	};

	class Enemy
		: public System
	{
	public:
		Enemy();
		Enemy(EnemyEntity entity, std::function<void(int)> healthChangedCallback);
		inline String SystemName() { return "Player Input System"; }

		void SpinUp() override;
		void WindDown() override;
		void Run() override;

		float getSpeed();
		void setSpeed(float speed);
		int getHealth();
		void setHealth(int health);
		void setHealthChangedCallback(std::function<void(int)> callback);

	private:
		EnemyEntity enemy;
		std::function<void(int)> healthChangedCallback;
	};

}
