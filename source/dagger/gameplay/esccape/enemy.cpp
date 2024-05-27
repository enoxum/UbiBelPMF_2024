#include "enemy.h"
#include <random>
#include <chrono>

#include "bullet.h" 

using namespace esccape;
using namespace dagger;

void esccape::EnemySystem::Run()
{
    //auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();
    auto view = Engine::Registry().view<Transform, Enemy, Sprite>();

    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    if (duration.count() > time + delta)
    {
        time += delta;
        CreateBullet(Vector2{ -200,0 }, Vector2{ 1, 0 }, 250.f);
    }
}

void esccape::EnemySystem::SpinUp() 
{
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

    time = duration.count();
}