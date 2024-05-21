#pragma once

#include "core/system.h"
#include "core/core.h"
#include "gameplay/common/particles.h"


using namespace dagger;

namespace academic_life
{
    struct ControllerMapping
    {
        EDaggerKeyboard leftKey = EDaggerKeyboard::KeyA;
        EDaggerKeyboard rightKey = EDaggerKeyboard::KeyD;

        Vector2 input{ 0, 0 };
    };

    struct AcademicPlayer
    {
        Float32 horzSpeed;
        void SetSpeedBasedOnHealth(int health, float TileSize);
    };

    void SetParticleSettings(common_res::ParticleSpawnerSettings& particleSettings, int currentHealth);

    class AcademicPlayerInputSystem
        : public System
    {
    public:
        inline String SystemName() { return "Academic Player Input System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:

        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
}
