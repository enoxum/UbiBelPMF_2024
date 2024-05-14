#include "academic_player.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/academic_life/academic_life_logic.h"

using namespace dagger;
using namespace academic_life;

void AcademicPlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&AcademicPlayerInputSystem::OnKeyboardEvent>(this);
}

void AcademicPlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&AcademicPlayerInputSystem::OnKeyboardEvent>(this);
}

void AcademicPlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
        if (kEvent_.key == ctrl_.leftKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.input.x = -1;
        }
        else if (kEvent_.key == ctrl_.leftKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
        {
            ctrl_.input.x = 0;
        }
        else if (kEvent_.key == ctrl_.rightKey && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
        {
            ctrl_.input.x = 1;
        }
        else if (kEvent_.key == ctrl_.rightKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
        {
            ctrl_.input.x = 0;
        }
    });
}

void AcademicPlayerInputSystem::Run()
{
    AcademicLifeFieldSettings fieldSettings;
    if (auto* ptr = Engine::GetDefaultResource<AcademicLifeFieldSettings>())
    {
        fieldSettings = *ptr;
    }

    Float32 bottomBoarderY = -fieldSettings.GetYBoarder();
    Float32 boarderX = fieldSettings.GetXBoarder();

    auto view = Engine::Registry().view<Transform, ControllerMapping, AcademicPlayer>();
    for (auto entity : view)
    {
        auto &t = view.get<Transform>(entity);
        auto &ctrl = view.get<ControllerMapping>(entity);
        auto &academic_player = view.get<AcademicPlayer>(entity);
        
        t.position.y = bottomBoarderY + 65.0f; 
                                //npr -235.0f;
        t.position.x += ctrl.input.x * academic_player.horzSpeed * Engine::DeltaTime();

        if (t.position.x > boarderX)
        {
            t.position.x = boarderX;
        }

        if (t.position.x < -boarderX)
        {
            t.position.x = -boarderX;
        }
    }
}

namespace academic_life
{
    void AcademicPlayer::SetSpeedBasedOnHealth(int health, float TileSize)
    {
        const float horz_speed_coef = (health + 60) / static_cast<float>(40);
        horzSpeed = TileSize * (6 + 2 * horz_speed_coef);
        if (health == -100)
            horzSpeed = TileSize * 6;
    }
}
