#include "enemy.h"
#include <random>

using namespace esccape;
using namespace dagger;

void esccape::EnemySystem::Run()
{
    //auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();
    auto view = Engine::Registry().view<Transform, Enemy, Sprite>();

    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& enemy = view.get<Enemy>(entity);
        auto& sprite = view.get<Sprite>(entity);
        //sprite.scale.y = -std::abs(sprite.scale.y);
        //auto& col = view.get<SimpleCollision>(entity);
    }
}