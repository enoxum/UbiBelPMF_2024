#pragma once

#include "core/system.h"
#include "core/core.h"
#include "core/engine.h"

using namespace dagger;

namespace dead_end
{
    struct DeadEndEnemy
    {
        bool is_enemy{ true };
        bool is_dead{ false };
        bool is_boss{ false };
        Float32 speed = 40.f;
        Vector3 direction{ 0, 0, 0 };
        Float32 cooldown = 0.f;
        Float32 maxCooldown = 0.5f;
        Float32 meleeDamage = 0.15f;
    };

    class DeadEndEnemyMoveSystem
            : public System
    {

        inline String SystemName() { return "Enemy Move System"; }

        void Run() override;
    };

    class DeadEndEnemyBulletCollisionSystem
        : public System
    {
        inline String SystemName() { return "Enemy Bullet Collision System"; }

        void Run() override;
    };

    class DeadEndEnemyObstacleCollisionSystem
        : public System
    {
        inline String SystemName() { return "Enemy Collision System"; }

        void Run() override;
    };
}
