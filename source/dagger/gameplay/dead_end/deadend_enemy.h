#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace dead_end
{
    struct DeadEndEnemy
    {
        bool is_enemy{ true };
        bool is_dead{ false };
        bool is_boss{ false };
        Float32 speed = 10.f;
        Vector3 direction{ 0, 0, 0 };
        int cooldown = 100;
        Float32 meleeDamage = 15.0f;
    };

    class DeadEndEnemySystem
            : public System
    {
    public:

        inline String SystemName() { return "Enemy System"; }

        void Run() override;
    };
}
