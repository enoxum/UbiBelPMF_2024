#include "gameplay/ping_pong/bullethell_bullet.h"
#include "core/system.h"
#include "core/core.h"
#include "core/game/transforms.h"
#include "core/engine.h"
#include "gameplay/ping_pong/pingpong_playerinput.h"
#include "gameplay/common/simple_collisions.h"
#include "core/graphics/sprite.h"


using namespace ping_pong;

void DamageSystem::Run() {
    auto view = Engine::Registry().view<Damageable>();

    for(auto& entity : view){

        auto& damageable = Engine::Registry().get<Damageable>(entity);

        if(damageable.lastDamaged > 0.f)
            damageable.lastDamaged -= Engine::DeltaTime();
        if(damageable.lastDamaged < 0.f){
            damageable.lastDamaged = 0.f;

            auto targetSprite = Engine::Registry().try_get<Sprite>(entity);
            if(targetSprite){
                targetSprite->color = {1,1,1, 1};
            }

        }
    }
}

void DamageSystem::OnDamageEvent(DamageEvent dEvent) {
    auto stats = Engine::Registry().try_get<StatsData>(dEvent.target);
    if(stats){
        Logger::info("Damage " + std::to_string(dEvent.damageAmount));
        stats->hp -= dEvent.damageAmount;
        Logger::info("HP: " + std::to_string(stats->hp));
        Engine::Registry().destroy(dEvent.owner);

        auto targetSprite = Engine::Registry().try_get<Sprite>(dEvent.target);
        auto targetDamageable = Engine::Registry().try_get<Damageable>(dEvent.target);
        if(targetSprite && targetDamageable){
            targetSprite->color = {1,0,0, 1};
            targetDamageable->lastDamaged = targetDamageable->hurtDuration;
        }
    }

}

void DamageSystem::SpinUp() {
    Engine::Dispatcher().sink<DamageEvent>().connect<&DamageSystem::OnDamageEvent>(this);
}

void DamageSystem::WindDown() {
    Engine::Dispatcher().sink<DamageEvent>().disconnect<&DamageSystem::OnDamageEvent>(this);
}

void BulletSystem::Run() {
    auto view = Engine::Registry().view<Bullet, Transform, SimpleCollision>();
    for(auto& bullet : view){
        auto& transform = Engine::Registry().get<Transform>(bullet);
        auto& bulletData = Engine::Registry().get<Bullet>(bullet);
        transform.position += bulletData.velocity * Engine::DeltaTime();
        auto& col = Engine::Registry().get<SimpleCollision>(bullet);
        if(col.colided && bulletData.owner != col.colidedWith){
            Engine::Dispatcher().trigger(DamageEvent{1,bullet, col.colidedWith});
        }
        if(glm::distance({0,0,0}, transform.position) > 400){
            Engine::Registry().destroy(bullet);
        }
    }
}





