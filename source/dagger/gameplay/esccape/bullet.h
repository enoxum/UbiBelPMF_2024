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
    struct Bullet {
        float speed;
        float damage;
        Vector2 direction;
    };

    class ShootingSystem
        : public System
    {
    public:
        inline String SystemName() { return "Shooting System"; }

        void Run() override;
    };

    void CreateBullet(Vector2 position_, Vector2 direction, float damage);
}

