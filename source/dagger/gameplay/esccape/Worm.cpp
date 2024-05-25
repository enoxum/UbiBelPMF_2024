#include "Worm.h"

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

        t.position += (worm.speed * Engine::DeltaTime());
    }
}