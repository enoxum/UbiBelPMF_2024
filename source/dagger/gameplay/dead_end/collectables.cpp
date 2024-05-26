#include "collectables.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "gameplay/common/simple_collisions.h"

#include <algorithm>
#include "deadend_health.h"

using namespace dagger;
using namespace dead_end;


void dead_end::GenerateCollectable(int x, int y, CollectableType type)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();

    auto& transform = reg.emplace<Transform>(entity);
    transform.position.x = static_cast<float>(x);
    transform.position.y = static_cast<float>(y);

    auto& collectable = reg.emplace<Collectable>(entity);
    collectable.type = type;
    collectable.value = (type == CollectableType::HealthPack) ? 50 : 1;

  
    auto& collision = reg.emplace<SimpleCollision>(entity);
    collision.size = { 32, 32 }; 
}

void dead_end::ApplyCollectableEffect(Collectable& collectable_, Health& health_)
{
    switch (collectable_.type)
    {
    case CollectableType::HealthPack:
        health_.currentHealth = std::min(health_.maxHealth, health_.currentHealth + collectable_.value);
        break;
    case CollectableType::WeaponUpgrade:
        //player.weaponType = player.weaponType + collectable_.value;
        break;
    }
}

void dead_end::HandleCollectableCollision (Collectable& collectable_, Health& health_)
{
    ApplyCollectableEffect(collectable_, health_);
}
