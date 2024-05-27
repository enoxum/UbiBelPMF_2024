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

	for (auto playerEntity : playerView)
	{

		auto& health = playerView.get<Health>(playerEntity);
		auto& t = playerView.get<Transform>(playerEntity);
		float healthRatio = 0.f;

		for (auto hudEntity : hudView)
		{

			auto& hud = hudView.get<HUD>(hudEntity);
			auto& hud_t = hudView.get<Transform>(hudEntity);
			auto& hud_s = hudView.get<Sprite>(hudEntity);


			healthRatio = static_cast<float>(health.currentHealth) / health.maxHealth;

			hud_t.position.x = t.position.x - hud.position.x;
			hud_t.position.y = t.position.y - hud.position.y;

			hud_s.size.x = hud.size.x * healthRatio;
			hud_s.size.y = hud.size.y;

			hud_s.size.x = std::clamp(hud_s.size.x, 0.0f, hud.size.x);
		}


	}

}
