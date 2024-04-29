#include "bober_kuvar_controller.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace bober_kuvar;

Float32 RoguelikeControllerSystem::player_speed = 100.0f;

void RoguelikeControllerSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&RoguelikeControllerSystem::OnKeyboardEvent>(this);
}

void RoguelikeControllerSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&RoguelikeControllerSystem::OnKeyboardEvent>(this);
}

void RoguelikeControllerSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
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

        if (ctrl_.input != Vector2{0, 0})
        {
            ctrl_.input *= 1.0f / sqrt(ctrl_.input.x * ctrl_.input.x +
                                      ctrl_.input.y * ctrl_.input.y);
        }
    });
}

void RoguelikeControllerSystem::Run()
{
    auto view = Engine::Registry().view<Transform, ControllerMapping>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);

        t.position.x += ctrl.input.x * player_speed * Engine::DeltaTime();
        t.position.y += ctrl.input.y * player_speed * Engine::DeltaTime();

        Logger::info("New position: ({}, {})", t.position.x, t.position.y);
    }
}
