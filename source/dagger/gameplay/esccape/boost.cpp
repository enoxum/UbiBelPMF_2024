#include <math.h>

#include "boost.h"

using namespace esccape;
using namespace dagger;

void esccape::CreateBoost(Vector2 position_, Vector2 direction, float speed)
{

    auto& reg = Engine::Instance().Registry();
    auto entity = reg.create();

    auto& boost = reg.emplace<Boost>(entity);
    boost.direction = direction;
    boost.speed = speed;

    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "Esccape:waterdrop");

    int boostSize = 40;
    float ratio = sprite.size.y / sprite.size.x;
    sprite.size = { boostSize, boostSize * ratio };

    auto& collision = reg.emplace<SimpleCollision>(entity);
    collision.size = sprite.size;

    auto& transform = reg.emplace<Transform>(entity);

    transform.position.x = position_.x;
    transform.position.y = position_.y;
    transform.position.z = 0.0f;
}

void esccape::BoostSystem::Run()
{
    auto view = Engine::Registry().view<Transform, Boost, Sprite, SimpleCollision>();

    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& boost = view.get<Boost>(entity);
        auto& sprite = view.get<Sprite>(entity);
        auto& col = view.get<SimpleCollision>(entity);

        t.position.x += boost.direction.x * boost.speed * Engine::DeltaTime();
        t.position.y += boost.direction.y * boost.speed * Engine::DeltaTime();
    }
}