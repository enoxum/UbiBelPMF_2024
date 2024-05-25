#pragma once

#include "core/system.h"
#include "core/core.h"
#include "gameplay/dead_end/deadend_player.h"

using namespace dagger;

namespace dead_end
{
    enum class CollectableType {
        HealthPack,
        WeaponUpgrade
    };

    struct Collectable {
        CollectableType type;
        int value;
        std::string sprite; // 
    };

    class CollectableSystem : public System
    {
    public:
        inline String SystemName() override { return "Collectable System"; }
        void Run() override;

    private:
        void GenerateCollectable(int x, int y, CollectableType type);
        void ApplyCollectableEffect(Player& player, Collectable& collectable);
        void HandleCollectableCollision(Player& player, Collectable& collectable);
    };
}
