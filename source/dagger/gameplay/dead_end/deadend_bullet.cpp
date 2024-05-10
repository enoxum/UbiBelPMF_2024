#include "gameplay/dead_end/deadend_bullet.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "gameplay/common/simple_collisions.h"

#include <math.h>


using namespace dagger;
using namespace dead_end;

void dead_end::CreateBullet(Vector2 position_, Vector2 target_, int weaponType_)
{

	auto& reg = Engine::Instance().Registry();
	auto entity = reg.create();

	auto& bullet = reg.emplace<Bullet>(entity);
	auto& sprite = reg.emplace<Sprite>(entity);
    // assign a sprite for bullets when found.

    float tileSize;


    if (weaponType_ == 1)
    {
        tileSize = 10.f;
        bullet.damage = 10;
    }
    else if (weaponType_ == 2)
    {
        tileSize = 20.f;
        bullet.damage = 20;
    }

    sprite.size.x = tileSize;
    sprite.size.y = tileSize;

    auto& collision = reg.emplace<SimpleCollision>(entity);
    collision.size.x = tileSize;
    collision.size.y = tileSize;

    auto& transform = reg.emplace<Transform>(entity);

    Float32 distanceX = target_.x - position_.x;
    Float32 distanceY = (target_.y * -1) - position_.y;
    Float32 ratio = distanceY / distanceX;
    ratio *= (ratio > 0) ? 1 : -1;

    Float32 speedXIntensity = sqrt((bullet.speed * bullet.speed) / (1 + ratio * ratio));
    bullet.speedX = speedXIntensity;
    bullet.speedY = speedXIntensity * ratio;

    transform.position.x = position_.x;
    transform.position.y = position_.y;
    transform.position.z = 0.0f;

    

}

void dead_end::ShootingSystem::Run()
{
    auto view = Engine::Registry().view<Transform, Bullet, Sprite, SimpleCollision>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& bullet = view.get<Bullet>(entity);
        auto& sprite = view.get<Sprite>(entity);
        auto& col = view.get<SimpleCollision>(entity);
        t.position.x += bullet.speedX * Engine::DeltaTime();
        t.position.y += bullet.speedY * Engine::DeltaTime();

        if (col.colided)
        {

            if (Engine::Registry().valid(col.colidedWith))
            {
                auto& reg = Engine::Instance().Registry();
                // colliding stuff here
            }
        }
    }
}
