#include "PlayerController.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace bober_game;

Float32 PlayerController::player_speed = 100.0f;

void PlayerController::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&PlayerController::OnKeyboardEvent>(this);
}

void PlayerController::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&PlayerController::OnKeyboardEvent>(this);
}

void PlayerController::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
        if (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held)
        {
            if (kEvent_.key == ctrl_.up_key)
                ctrl_.input.y = 1;
            if (kEvent_.key == ctrl_.down_key)
                ctrl_.input.y = -1;

            if (kEvent_.key == ctrl_.left_key)
                ctrl_.input.x = -1;
            if (kEvent_.key == ctrl_.right_key)
                ctrl_.input.x = 1;
        }
        else if (kEvent_.action == EDaggerInputState::Released)
        {
            if(kEvent_.key == ctrl_.up_key || kEvent_.key == ctrl_.down_key)
                ctrl_.input.y = 0;

            if (kEvent_.key == ctrl_.left_key || kEvent_.key == ctrl_.right_key)
                ctrl_.input.x = 0;
        }
    });
}

void PlayerController::Run()
{
    auto view = Engine::Registry().view<Transform, ControllerMapping>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);

        if (ctrl.input == Vector2{0.0f, 0.0f})
            break;
        double normalized = 1 / sqrt(ctrl.input.x * ctrl.input.x + ctrl.input.y * ctrl.input.y);

        t.position.x += normalized * ctrl.input.x * player_speed * Engine::DeltaTime();
        t.position.y += normalized * ctrl.input.y * player_speed * Engine::DeltaTime();
    }
}
