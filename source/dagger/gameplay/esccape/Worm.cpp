#include "Worm.h"
#include <random>

using namespace esccape;
using namespace dagger;

void esccape::WormSystem::Run()
{
    //auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();
    auto view = Engine::Registry().view<Transform, Worm>();
    //auto view = Engine::Registry().view<Transform, Worm>();

    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& worm = view.get<Worm>(entity);
        //auto& col = view.get<SimpleCollision>(entity);
        if (t.position.y < -worm.startingPos.y - 10) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(-(worm.startingPos.y), worm.startingPos.y);
            int x_pos = dis(gen);
            t.position.x = x_pos;
            t.position.y = worm.startingPos.y;
        }

        t.position += (worm.speed * Engine::DeltaTime());
    }
}