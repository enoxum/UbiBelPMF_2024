#include "PlayerController.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace bober_game;

double PlayerController::playerSpeed = 1000.0;

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
            if (kEvent_.key == ctrl_.upKey)
                ctrl_.input.y = 1;
            if (kEvent_.key == ctrl_.downKey)
                ctrl_.input.y = -1;

            if (kEvent_.key == ctrl_.leftKey)
                ctrl_.input.x = -1;
            if (kEvent_.key == ctrl_.rightKey)
                ctrl_.input.x = 1;
        }
        else if (kEvent_.action == EDaggerInputState::Released)
        {
            if(kEvent_.key == ctrl_.upKey || kEvent_.key == ctrl_.downKey)
                ctrl_.input.y = 0;

            if (kEvent_.key == ctrl_.leftKey || kEvent_.key == ctrl_.rightKey)
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

        t.position.x += normalized * ctrl.input.x * playerSpeed * Engine::DeltaTime();
        t.position.y += normalized * ctrl.input.y * playerSpeed * Engine::DeltaTime();
    }
}
