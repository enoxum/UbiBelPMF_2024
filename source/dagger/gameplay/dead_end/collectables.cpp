#include "collectables.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "gameplay/common/simple_collisions.h"

using namespace dagger;
using namespace dead_end;

void CollectableSystem::Run()
{
    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    auto viewPlayers = reg.view<Player>();
    auto viewCollisions = reg.view<SimpleCollision>();

    for (auto playerEntity : viewPlayers)
    {
        auto& player = viewPlayers.get(playerEntity);

        for (auto entity : viewCollisions)
        {
            auto& col = viewCollisions.get(entity);

            if (col.colided)
            {
                if (reg.has<Collectable>(col.colidedWith))
                {
                    auto& collectable = reg.get<Collectable>(col.colidedWith);
                    HandleCollectableCollision(player, collectable);
                    reg.destroy(col.colidedWith);
                }
            }
        }
    }
}

void CollectableSystem::GenerateCollectable(int x, int y, CollectableType type)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();

    auto& transform = reg.emplace<Transform>(entity);
    transform.position.x = static_cast<float>(x);
    transform.position.y = static_cast<float>(y);

    auto& collectable = reg.emplace<Collectable>(entity);
    collectable.type = type;
    collectable.value = (type == CollectableType::HealthPack) ? 50 : 1;
    collectable.sprite = (type == CollectableType::HealthPack) ? "health_pack_sprite" : "weapon_upgrade_sprite";

  
    auto& collision = reg.emplace<SimpleCollision>(entity);
    collision.size = { 32, 32 }; 
}

void CollectableSystem::ApplyCollectableEffect(Player& player, Collectable& collectable)
{
    switch (collectable.type)
    {
    case CollectableType::HealthPack:
        player.health = std::min(player.maxHealth, player.health + collectable.value);
        break;
    case CollectableType::WeaponUpgrade:
        player.weaponType = player.weaponType + collectable.value;
        break;
    }
}

void CollectableSystem::HandleCollectableCollision(Player& player, Collectable& collectable)
{
    ApplyCollectableEffect(player, collectable);
}
