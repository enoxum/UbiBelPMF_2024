#include "deadend_playerinput.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace dead_end;

void dead_end::DeadEndPlayerInputSystem::SpinUp()
{
	Engine::Dispatcher().sink<KeyboardEvent>().connect<&DeadEndPlayerInputSystem::OnKeyboardEvent>(this);
}

void dead_end::DeadEndPlayerInputSystem::WindDown()
{
	Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&DeadEndPlayerInputSystem::OnKeyboardEvent>(this);
}

void dead_end::DeadEndPlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
        {
            // moving
            if (kEvent_.key == ctrl_.up_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.input.y = 1;
            }
            else if (kEvent_.key == ctrl_.up_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y > 0)
            {
                ctrl_.input.y = 0;
            }
            else if (kEvent_.key == ctrl_.down_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.input.y = -1;
            }
            else if (kEvent_.key == ctrl_.down_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y < 0)
            {
                ctrl_.input.y = 0;
            }
            else if (kEvent_.key == ctrl_.left_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.input.x = -1;
            }
            else if (kEvent_.key == ctrl_.left_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
            {
                ctrl_.input.x = 0;
            }
            else if (kEvent_.key == ctrl_.right_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.input.x = 1;
            }
            else if (kEvent_.key == ctrl_.right_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
            {
                ctrl_.input.x = 0;
            }

            // shooting
            if (kEvent_.key == ctrl_.shoot_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.shooting = true;
            }
            else if (kEvent_.key == ctrl_.shoot_key && kEvent_.action == EDaggerInputState::Released && ctrl_.shooting == true)
            {
                ctrl_.shooting = false;
            }
        });
}



void dead_end::DeadEndPlayerInputSystem::Run()
{

    auto view = Engine::Registry().view<Transform, ControllerMapping>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);

        t.position.y += ctrl.input.y * Engine::DeltaTime(); // * player.speed.x
        t.position.x += ctrl.input.x * Engine::DeltaTime(); // * player.speed.y
    }
}

