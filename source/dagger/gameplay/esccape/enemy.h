#pragma once

#include "core/system.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "gameplay/common/simple_collisions.h"

using namespace dagger;

namespace esccape
{
    struct Enemy {
        Vector2 pos = { 0,0 };
    };

    class EnemySystem
        : public System
    {
    public:
        inline String SystemName() { return "Enemy System"; }
        SimpleCollision* collision;
        Transform* transform;

        long long time = 0;
        const long long delta = 4000;

        void Run() override;
        void SpinUp() override;
    };
}