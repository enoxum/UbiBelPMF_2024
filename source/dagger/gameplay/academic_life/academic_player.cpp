#include "academic_player.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include "core/graphics/animations.h"

#include "gameplay/academic_life/academic_life_logic.h"
#include "health.h"
#include <gameplay/common/particles.h>


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
                Engine::Registry().view<Animator>().each([&](Animator& animator_)
                    {
                        AnimatorPlay(animator_, "AcademicLife:RUN_LEFT");
                    });
            }
            else if (kEvent_.key == ctrl_.leftKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
            {
                ctrl_.input.x = 0;
                Engine::Registry().view<Animator>().each([&](Animator& animator_)
                    {
                        AnimatorPlay(animator_, "AcademicLife:IDLE");
                        
                    });
            }
            else if (kEvent_.key == ctrl_.rightKey && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.input.x = 1;
                Engine::Registry().view<Animator>().each([&](Animator& animator_)
                    {
                        AnimatorPlay(animator_, "AcademicLife:RUN_RIGHT");
                    });
            }
            else if (kEvent_.key == ctrl_.rightKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
            {
                ctrl_.input.x = 0;
                Engine::Registry().view<Animator>().each([&](Animator& animator_)
                    {
                        AnimatorPlay(animator_, "AcademicLife:IDLE");
                    });
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

    auto view = Engine::Registry().view<Transform, ControllerMapping, AcademicPlayer, common_res::ParticleSpawnerSettings>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);
        auto& academic_player = view.get<AcademicPlayer>(entity);
        auto& particleSettings = view.get<common_res::ParticleSpawnerSettings>(entity);

        Health& health = Health::Instance();
        int currentHealth = health.GetValue();

        academic_player.SetSpeedBasedOnHealth(currentHealth, 20.f); //20.f je TileSize TO DO
        SetParticleSettings(particleSettings, currentHealth);
        common_res::ParticleSystem::UpdateParticleSpawnerSettings(entity, particleSettings);


        t.position.y = bottomBoarderY + 65.0f;
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

    void SetParticleSettings(common_res::ParticleSpawnerSettings& particleSettings, int currentHealth)
    {
        if (currentHealth < -60) {
            particleSettings.Setup(0.05f, { 3.f, 3.f }, { -0.2f, -1.4f }, { 0.2f, -0.6f },
                { 0,0,0,1 }, { 0.2f,0.2f,0.2f,1 }, "EmptyWhitePixel");
        }
        else if (currentHealth < -20) {
            particleSettings.Setup(0.05f, { 2.f, 2.f }, { -0.2f, -1.4f }, { 0.2f, -0.6f },
                { 0.6f,0.6f,0.6f,1 }, { 1,1,1,1 }, "EmptyWhitePixel");
        }
        else if (currentHealth < 20) {
            particleSettings.Setup(0.0f, { 0.f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f },
                { 0,0,0,1 }, { 0,0,0,0 }, "EmptyWhitePixel");
        }
        else if (currentHealth < 60) {
            particleSettings.Setup(0.05f, { 1.5f, 1.5f }, { -0.2f, -1.4f }, { 0.2f, -0.6f },
                { 0,0.6f,0.2f,1 }, { 0,0.8f,0.2f,1 }, "EmptyWhitePixel");
        }
        else if (currentHealth < 100) {
            particleSettings.Setup(0.03f, { 3.f, 3.f }, { -0.2f, -2.4f }, { 0.2f, -1.6f },
                { 0.2f,0.8f,0.2f,1 }, { 0.2f,1,0.2f,1 }, "EmptyWhitePixel");
        }
        else {
            particleSettings.Setup(0.05f, { 2.f, 2.f }, { -0.2f, -1.4f }, { 0.2f, -0.6f },
                { 0,0.5f,1,1 }, { 0,0.6f,1,1 }, "EmptyWhitePixel");
        }
    }
}