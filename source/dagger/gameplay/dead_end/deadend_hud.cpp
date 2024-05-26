#include "deadend_hud.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"
#include "core/input/inputs.h"

#include "core/game/transforms.h"
#include "deadend_health.h"
#include "deadend_player.h"

using namespace dagger;
using namespace dead_end;

void dead_end::DeadEndHUDSystem::Run()
{
	auto hudView = Engine::Registry().view<HUD, Transform, Sprite>();
	auto playerView = Engine::Registry().view<Health, Player, Transform>();

	for (auto entity : hudView)
	{

		auto& hud = hudView.get<HUD>(entity);
		auto& hud_t = hudView.get<Transform>(entity);
		auto& hud_s = hudView.get<Sprite>(entity);

		for (auto playerEntity : playerView)
		{

			auto& health = playerView.get<Health>(playerEntity);
			auto& t = playerView.get<Transform>(playerEntity);

			float healthRatio = static_cast<float>(health.currentHealth) / health.maxHealth;
			hud.size.y *= healthRatio;

			hud_t.position.x = t.position.x - hud.position.x;
			hud_t.position.y = t.position.y - hud.position.y;

			
		}

		hud_s.size = hud.size;



	}

}
