#include "Worm.h"
#include <random>

using namespace esccape;
using namespace dagger;

void esccape::WormSystem::Run()
{
    //auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();
    auto view = Engine::Registry().view<Transform, Worm, Sprite>();
    //auto view = Engine::Registry().view<Transform, Worm>();

    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& worm = view.get<Worm>(entity);
        auto& sprite = view.get<Sprite>(entity);
        //sprite.scale.y = -std::abs(sprite.scale.y);
        //auto& col = view.get<SimpleCollision>(entity);
        if (worm.spawnEdge == 0 && t.position.y < -worm.startingYTop - 10) {
            //printf("Usao u if\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            //std::random_device rd;
            //std::mt19937 gen(rd());
            //std::uniform_int_distribution<> dis(-(worm.startingPos.y), worm.startingPos.y);
            //int x_pos = dis(gen);
            //t.position.x = x_pos;
            //t.position.y = worm.startingPos.y;
            int randomEdge = rand() % 4;
            int pos = generateSpawnPoint(randomEdge, worm);
            setPosition(randomEdge, pos, t, worm, sprite);
            worm.spawnEdge = randomEdge;
        }
        else if (worm.spawnEdge == 1 && t.position.x < -worm.startingXRight - 10) {
            int randomEdge = rand() % 4;
            int pos = generateSpawnPoint(randomEdge, worm);
            setPosition(randomEdge, pos, t, worm, sprite);
            worm.spawnEdge = randomEdge;
        }
        else if (worm.spawnEdge == 2 && t.position.y > -worm.startingYBottom + 10) {
            int randomEdge = rand() % 4;
            int pos = generateSpawnPoint(randomEdge, worm);
            setPosition(randomEdge, pos, t, worm, sprite);
            worm.spawnEdge = randomEdge;
        }
        else if (worm.spawnEdge == 3 && t.position.x > -worm.startingXLeft + 10) {
            int randomEdge = rand() % 4;
            int pos = generateSpawnPoint(randomEdge, worm);
            setPosition(randomEdge, pos, t, worm, sprite);
            worm.spawnEdge = randomEdge;
        }

        t.position += (worm.speed * Engine::DeltaTime());
    }
}

int esccape::generateSpawnPoint(int edge, Worm& worm) {
    std::random_device rd;
    std::mt19937 gen(rd());
    int randomEdge = rand() % 4;
    int x_pos, y_pos;
    std::uniform_int_distribution<> dis0(worm.minimumX, worm.maximumX);
    std::uniform_int_distribution<> dis1(worm.minimumY, worm.maximumY);
    std::uniform_int_distribution<> dis2(worm.minimumX, worm.maximumX);
    std::uniform_int_distribution<> dis3(worm.minimumY, worm.maximumY);
    switch (randomEdge) {
        case 0:
            x_pos = dis0(gen);
            return x_pos;
        case 1: 
            y_pos = dis1(gen);
            return y_pos;
        case 2:
            x_pos = dis2(gen);
            return x_pos;
        case 3:
            x_pos = dis3(gen);
            return x_pos;
    }
    
    return -1;
}

void esccape::setPosition(int edge, int pos, Transform& t, Worm& worm, Sprite& sprite) {
    switch (edge) {
        case 0:
            t.position.x = pos;
            t.position.y = worm.startingYTop;
            sprite.rotation = 0;
            worm.speed = { 0, -100, 0 };
            break;
        case 1:
            t.position.x = worm.startingXRight;
            t.position.y = pos;
            sprite.rotation = 270;
            worm.speed = { -100, 0, 0 };
            break;
        case 2:
            t.position.x = pos;
            t.position.y = worm.startingYBottom;
            sprite.rotation = 180;
            worm.speed = { 0, 100, 0 };
            break;
        case 3:
            t.position.x = worm.startingXLeft;
            t.position.y = pos;
            sprite.rotation = 90;
            worm.speed = { 100, 0, 0 };
            break;
    }
}