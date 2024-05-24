#include "deadend_crosshair.h"

#include "core/input/inputs.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/window.h"
#include "core/graphics/sprite.h"
#include "core/input/inputs.h"
#include <core/game/transforms.h>


void dead_end::AimSystem::Run()
{

    auto* camera = Engine::GetDefaultResource<Camera>();
    auto cursor = dagger::Input::CursorPositionInWindow();
    Vector2 screenCenter{ camera->size.x / 2, camera->size.y / 2 };

    Vector2 relativePosition = cursor - screenCenter;

    Vector2 crosshairPosition;
    crosshairPosition.x = camera->position.x + relativePosition.x;
    crosshairPosition.y = camera->position.y - relativePosition.y;

    auto view = Engine::Registry().view<Transform, Sprite, Aim>();
    for (auto entity : view)
    {
        auto& transform = view.get<Transform>(entity);
        transform.position = { crosshairPosition.x, crosshairPosition.y, 0.0f };
    }
}





