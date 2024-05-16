#include "gameplay/dead_end/deadend_bullet.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "gameplay/common/simple_collisions.h"

#include <math.h>


#include "gameplay/dead_end/deadend_obstacle.h"

using namespace dagger;
using namespace dead_end;

void dead_end::CreateBullet(Vector2 position_, Vector2 target_, int weaponType_)
{

	auto& reg = Engine::Instance().Registry();
	auto entity = reg.create();

	auto& bullet = reg.emplace<Bullet>(entity);
	auto& sprite = reg.emplace<Sprite>(entity);
    // assign a sprite for bullets when found and according to a weapon type.
    AssignSprite(sprite, "dead_end:Bullet:bullet_pistol");

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
    Float32 distanceY = target_.y  - position_.y;


    auto norm = sqrt(distanceX * distanceX + distanceY * distanceY);
    bullet.direction.x = distanceX / norm;
    bullet.direction.y = distanceY / norm;


    transform.position.x = position_.x ;
    transform.position.y = position_.y ;
    transform.position.z = 0.0f;

    

}

void dead_end::ShootingSystem::Run()
{
    auto view = Engine::Registry().view<Transform, Bullet, Sprite, SimpleCollision>();
    auto viewCollisions = Engine::Registry().view<Transform, DeadEndObstacle>();

    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& bullet = view.get<Bullet>(entity);
        auto& sprite = view.get<Sprite>(entity);
        auto& col = view.get<SimpleCollision>(entity);

        
        t.position.x += bullet.direction.x * bullet.speed * Engine::DeltaTime();
        t.position.y += bullet.direction.y * bullet.speed * Engine::DeltaTime();


        if (col.colided)
        {

            if (Engine::Registry().valid(col.colidedWith) && viewCollisions.contains(col.colidedWith))
            {

                Engine::Registry().destroy(entity);
              
            }

        }
    }
}
