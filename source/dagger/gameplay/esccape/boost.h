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
    struct Boost {
        float speed;
        float health;
        Vector2 direction;
        int id;
    };

    class BoostSystem
        : public System
    {
    public:
        inline String SystemName() { return "Boost System"; }

        void Run() override;
    };

    void CreateBoost(int id, Vector2 position_, Vector2 direction, float speed);
}

