#include <math.h>

#include "bullet.h"

using namespace esccape;
using namespace dagger;

void esccape::CreateBullet(Vector2 position_, Vector2 direction, float speed)
{

    auto& reg = Engine::Instance().Registry();
    auto entity = reg.create();

    auto& bullet = reg.emplace<Bullet>(entity);
    bullet.direction = direction;
    bullet.speed = speed;

    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "Esccape:bullet-sprite");

    int bulletSize = 25;
    float ratio = sprite.size.y / sprite.size.x;
    sprite.size = { bulletSize, bulletSize * ratio };

    auto& collision = reg.emplace<SimpleCollision>(entity);
    collision.size = sprite.size;

    auto& transform = reg.emplace<Transform>(entity);

    transform.position.x = position_.x;
    transform.position.y = position_.y;
    transform.position.z = 0.0f;
}

void esccape::ShootingSystem::Run()
{
    auto view = Engine::Registry().view<Transform, Bullet, Sprite, SimpleCollision>();

    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& bullet = view.get<Bullet>(entity);
        auto& sprite = view.get<Sprite>(entity);
        auto& col = view.get<SimpleCollision>(entity);

        t.position.x += bullet.direction.x * bullet.speed * Engine::DeltaTime();
        t.position.y += bullet.direction.y * bullet.speed * Engine::DeltaTime();
    }
}