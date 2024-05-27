#pragma once

#include "core/system.h"
#include "core/core.h"
#include "gameplay/dead_end/deadend_player.h"
#include "deadend_health.h"

using namespace dagger;

namespace dead_end
{
    enum class CollectableType {
        HealthPack,
        WeaponUpgrade
    };

    struct Collectable {
        CollectableType type;
        Float32 value;
    };

    class CollectableSystem : public System
    {
    public:
        inline String SystemName() override { return "Collectable System"; }
        
    };
    
    void GenerateCollectable(int x, int y, CollectableType type);
    void ApplyCollectableEffect(Collectable& collectable_, Health& health_);
    void HandleCollectableCollision(Collectable& collectable_, Health& health_);
    
}
