#include "deadend_crosshair.h"

#include "core/input/inputs.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/window.h"
#include "core/graphics/sprite.h"
#include "core/input/inputs.h"


void dead_end::AimSystem::Run()
{

	auto view = Engine::Registry().view<Aim>();
	auto cursorPos = dagger::Input::CursorPositionInWorld();

	for (auto entity : view)
	{
		auto& a = view.get<Aim>(entity);

		auto& sprite = Engine::Registry().get<Sprite>(a.crosshairSprite);

		sprite.position.x = cursorPos.x + a.playerDistance;
		sprite.position.y = cursorPos.y + a.playerDistance;

		sprite.scale = { 1,1 };
	}

}




