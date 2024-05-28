#include "Worm.h"
#include <random>

using namespace esccape;
using namespace dagger;

void esccape::WormSystem::Run()
{
    auto view = Engine::Registry().view<Transform, Worm, Sprite, SimpleCollision>();

    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& worm = view.get<Worm>(entity);
        auto& sprite = view.get<Sprite>(entity);
        auto& col = view.get<SimpleCollision>(entity);
        if (worm.spawnEdge == 0 && t.position.y < -worm.startingYTop - 10) {
            int randomEdge = rand() % 4;
            int pos = generateSpawnPoint(randomEdge, worm);
            setPosition(randomEdge, pos, t, worm, sprite, col);
            worm.spawnEdge = randomEdge;
            worm.level++;
        }
        else if (worm.spawnEdge == 1 && t.position.x < -worm.startingXRight - 10) {
            int randomEdge = rand() % 4;
            int pos = generateSpawnPoint(randomEdge, worm);
            setPosition(randomEdge, pos, t, worm, sprite, col);
            worm.spawnEdge = randomEdge;
            worm.level++;
        }
        else if (worm.spawnEdge == 2 && t.position.y > -worm.startingYBottom + 10) {
            int randomEdge = rand() % 4;
            int pos = generateSpawnPoint(randomEdge, worm);
            setPosition(randomEdge, pos, t, worm, sprite, col);
            worm.spawnEdge = randomEdge;
            worm.level++;
        }
        else if (worm.spawnEdge == 3 && t.position.x > -worm.startingXLeft + 10) {
            int randomEdge = rand() % 4;
            int pos = generateSpawnPoint(randomEdge, worm);
            setPosition(randomEdge, pos, t, worm, sprite, col);
            worm.spawnEdge = randomEdge;
            worm.level++;
        }

        t.position += (worm.speed * Engine::DeltaTime());
    }
}

int esccape::generateSpawnPoint(int edge, Worm& worm) {
    std::random_device rd;
    std::mt19937 gen(rd());
    int x_pos, y_pos;
    std::uniform_int_distribution<> dis0(worm.minimumX, worm.maximumX);
    std::uniform_int_distribution<> dis1(worm.minimumY, worm.maximumY);
    std::uniform_int_distribution<> dis2(worm.minimumX, worm.maximumX);
    std::uniform_int_distribution<> dis3(worm.minimumY, worm.maximumY);

    switch (edge) {
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
            y_pos = dis3(gen);
            return y_pos;
    }
    
    return -1;
}

void esccape::setPosition(int edge, int pos, Transform& t, Worm& worm, Sprite& sprite, SimpleCollision& collision) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disDistance(400, 1200);
    std::uniform_int_distribution<> disSpeed(100, 200);
    int speed = disSpeed(gen);
    switch (edge) {
        case 0:
            t.position.x = pos;
            t.position.y = worm.startingYTop + disDistance(gen);
            sprite.rotation = 0;
            collision.size = { sprite.size.x, sprite.size.y };
            worm.speed = { 0, -(speed + worm.level * 30), 0 };
            break;
        case 1:
            t.position.x = worm.startingXRight + disDistance(gen);
            t.position.y = pos;
            sprite.rotation = 270;
            collision.size = { sprite.size.y, sprite.size.x };
            worm.speed = { -(speed + worm.level * 30), 0, 0 };
            break;
        case 2:
            t.position.x = pos;
            t.position.y = worm.startingYBottom - disDistance(gen);
            sprite.rotation = 180;
            collision.size = { sprite.size.x, sprite.size.y };
            worm.speed = { 0, speed + worm.level * 30, 0 };
            break;
        case 3:
            t.position.x = worm.startingXLeft - disDistance(gen);
            t.position.y = pos;
            sprite.rotation = 90;
            collision.size = { sprite.size.y, sprite.size.x };
            worm.speed = { speed + worm.level * 30, 0, 0 };
            break;
    }
}

void esccape::CreateNWorms(int n, int zPos, int screenWidth, int screenHeight) {
    for (int i = 0; i < n; i++)
        CreateWorm(zPos, screenWidth, screenHeight);
}

void esccape::CreateWorm(int zPos, int screenWidth, int screenHeight) {
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "Esccape:crv");
    int wormSize = 40;
    float ratio = sprite.size.y / sprite.size.x;
    sprite.size = { wormSize, wormSize * ratio };

    auto& worm = reg.emplace<Worm>(entity);

    auto& transform = reg.emplace<Transform>(entity);

    auto& collision = reg.emplace<SimpleCollision>(entity);

    worm.startingYTop = screenHeight / 2 + 50;
    worm.startingXRight = screenWidth / 2 + 30;
    worm.startingYBottom = -(screenHeight / 2 + 50);
    worm.startingXLeft = -(screenWidth / 2 + 30);
    worm.minimumX = -350; // 30
    worm.maximumX = 350;
    worm.minimumY = -250;
    worm.maximumY = 250;
    worm.level = 0;

    worm.scaleX = sprite.scale.x;
    worm.scaleY = sprite.scale.y;
    
    SpawnWorm(worm, transform, sprite, collision);
}

void esccape::SpawnWorm(Worm& worm, Transform& t, Sprite& sprite, SimpleCollision& collision) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disSpeed(100, 200);
    std::uniform_int_distribution<> disX(worm.minimumX, worm.maximumX);
    std::uniform_int_distribution<> disY(worm.minimumY, worm.maximumY);
    std::uniform_int_distribution<> disDistance(400, 1200);
    int edge = rand() % 4;

    int speed = disSpeed(gen);

    switch (edge) {
    case 0:
        t.position.x = disX(gen);
        t.position.y = worm.startingYTop + disDistance(gen);
        sprite.rotation = 0;
        collision.size = { sprite.size.x, sprite.size.y };
        worm.speed = { 0, -speed, 0 };
        break;
    case 1:
        t.position.x = worm.startingXRight + disDistance(gen);
        t.position.y = disY(gen);
        sprite.rotation = 270;
        collision.size = { sprite.size.y, sprite.size.x };
        worm.speed = { -speed, 0, 0 };
        break;
    case 2:
        t.position.x = disX(gen);
        t.position.y = worm.startingYBottom - disDistance(gen);
        sprite.rotation = 180;
        collision.size = { sprite.size.x, sprite.size.y };
        worm.speed = { 0, speed, 0 };
        break;
    case 3:
        t.position.x = worm.startingXLeft - disDistance(gen);
        t.position.y = disY(gen);
        sprite.rotation = 90;
        collision.size = { sprite.size.y, sprite.size.x };
        worm.speed = { speed, 0, 0 };
        break;
    }
}