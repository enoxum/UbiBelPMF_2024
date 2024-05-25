#pragma once
#include "core/system.h"
#include "core/engine.h"
#include "core/core.h"

using namespace dagger;

#include "core/graphics/sprite.h"

namespace dead_end
{
    struct Health
    {
        int currentHealth;
        int maxHealth;
    };

    class HealthSystem : public System
    {
    public:
        void Run() override;
    };
}
