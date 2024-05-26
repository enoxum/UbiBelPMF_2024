#pragma once
#include "core/system.h"
#include "core/engine.h"
#include "core/core.h"

#include "core/graphics/sprite.h"
using namespace dagger;


namespace dead_end
{
    struct Health
    {
        int currentHealth;
        int maxHealth;
    };

    class HealthSystem 
        : public System
    {
        inline String SystemName() override { return "Health system"; }
        
    };
}
