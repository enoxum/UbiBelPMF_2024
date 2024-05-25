#include "deadend_health.h"
#include "gameplay/dead_end/deadend_player.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

using namespace dagger;
using namespace dead_end;

void HealthSystem::Run()
{
    auto entity = Engine::Registry().view<Transform, Sprite, Player, Health>().front();

    auto& health = Engine::Registry().get<Health>(entity);
    auto& sprite = Engine::Registry().get<Sprite>(entity);
    auto& transform = Engine::Registry().get<Transform>(entity);

   
    float healthPercentage = static_cast<float>(health.currentHealth) / static_cast<float>(health.maxHealth);

    // size changing
    sprite.size.x = healthPercentage * 160.f; // 160 je maksimalna širina bara
    sprite.size.y = 32.f;

    // color changing
    if (healthPercentage >= 0.70f)
    {
        AssignSprite(sprite, "dead_end:Health:health_green");
    }
    else if (healthPercentage >= 0.40f)
    {
        AssignSprite(sprite, "dead_end:Health:health_orange");
    }
    else
    {
        AssignSprite(sprite, "dead_end:Health:health_red");
    }

    // bar position
    transform.position.x = -300.f;
    transform.position.y = -250.f;
}
