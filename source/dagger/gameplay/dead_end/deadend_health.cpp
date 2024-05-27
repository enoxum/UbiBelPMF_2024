#include "deadend_health.h"
#include "gameplay/dead_end/deadend_player.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "deadend_wavestruct.h"
#include "deadend_enemy.h"

#include <iostream>
using namespace dagger;
using namespace dead_end;

void dead_end::HealthSystem::Run()
{

	auto playerView = Engine::Registry().view<Player, Health>();
	auto enemyView = Engine::Registry().view<DeadEndEnemy, Health>();

	auto playerEntity = playerView.front();
	
	auto& playerHealth = playerView.get<Health>(playerEntity);

		if (playerHealth.currentHealth <= 0.f)
		{
			//exit(1);
			dead_end::gameOver();
			// game over :(
		}

	for (auto enemyEntity : enemyView)
	{
		auto& enemy = enemyView.get<DeadEndEnemy>(enemyEntity);

		if (enemy.is_boss)
		{
			auto& health = enemyView.get<Health>(enemyEntity);
			if (health.currentHealth <= 0.f)
			{
				playerHealth.currentHealth = 0.f;
				std::cout << "GAME OVER" << std::endl;

				//dead_end::gameOver();
				// game over :)
			}
		}
	}
}
