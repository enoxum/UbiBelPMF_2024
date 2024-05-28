#include "enemy.h"
#include <random>
#include <chrono>

#include "bullet.h" 

using namespace esccape;
using namespace dagger;

void esccape::EnemySystem::Run()
{
    auto view = Engine::Registry().view<Transform, Enemy, Sprite>();
    
    for (auto entity : view) 
    {
        auto& t = view.get<Transform>(entity);
        auto& s = view.get<Sprite>(entity);

        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        
        if (duration.count() > time + delta)
        {
            time += delta;
            CreateBullet(Vector2{t.position.x, t.position.y + s.size.y / 2.5f - 5.f}, Vector2{ 1, 0 }, 500.f);
        }
    }
}

void esccape::EnemySystem::SpinUp() 
{
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

    time = duration.count();
}