#include "machine.h"
#include <random>
#include <chrono>

#include "boost.h" 

using namespace esccape;
using namespace dagger;

void esccape::MachineSystem::Run()
{
    //auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();
    auto view = Engine::Registry().view<Transform, Machine, Sprite>();

    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& s = view.get<Sprite>(entity);

        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        int x_rnd = -200 + rand() % 201;
        int y_rnd = rand() % (-201);

        //printf("******* x_rnd=%d *** y_rnd=%d\n\n\n", x_rnd, y_rnd);

        if (duration.count() > time + delta)
        {
            time += delta;
            CreateBoost(Vector2{ t.position.x, t.position.y + s.size.y / 2.5f }, Vector2{ x_rnd, y_rnd }, 2.f);
        }
    }
}

void esccape::MachineSystem::SpinUp()
{
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

    time = duration.count();
}