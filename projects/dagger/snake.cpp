#include "snake.h"
#include "snake_playerinput.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "gameplay/common/simple_collisions.h"
#include <iostream>
#include "red_snake_main.h"

using namespace dagger;
using namespace red_snake;

void RedSnakeSystem::SpinUp()
{
    snakeSegments.clear();
}

void RedSnakeSystem::WindDown()
{
    snakeSegments.clear();
}

void RedSnakeSystem::AddSegment()
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();

    auto& transform = reg.emplace<Transform>(entity);
    if (!snakeSegments.empty())
    {
        auto& lastTransform = reg.get<Transform>(snakeSegments.back());
        transform.position = lastTransform.position - Vector3(20.0f, 0, 0);  // Postavi novi segment iza poslednjeg
    }

    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "RedSnake:body");
    sprite.size = Vector2(1, 1) * 20.0f;

    auto& snakeBody = reg.emplace<SnakeBody>(entity);
    snakeSegments.push_back(entity);
}


void RedSnakeSystem::Run()
{
    auto& reg = Engine::Registry();
    auto viewCollisions = reg.view<Transform, SimpleCollision>();
    auto view = reg.view<SnakeSegment, Transform, SimpleCollision>();

    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& segment = view.get<SnakeSegment>(entity);
        auto& col = view.get<SimpleCollision>(entity);

        if (col.colided)
        {
            if (reg.valid(col.colidedWith))
            {
                SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
                Transform& transform = viewCollisions.get<Transform>(col.colidedWith);

                if (reg.has<Food>(col.colidedWith))
                {
                    segment.grow = true;
                    reg.destroy(col.colidedWith);  // Uništi hranu
                    std::srand(static_cast<unsigned>(std::time(nullptr)));
                    int foodX = std::rand() % 24 + 1;
                    int foodY = std::rand() % 18 + 1;
                    CreateFood(20.0f, ColorRGBA(1, 0, 0, 1), { foodX, foodY, 1.0f });
                }
                else if (reg.has<SnakeSegment>(col.colidedWith))
                {
                    segment.alive = false;
                }

                col.colided = false;
            }
        }
        else
        {
            t.position += segment.speed * Engine::DeltaTime() ;
        }
    }

    for (auto entity : view)
    {
        auto& segment = view.get<SnakeSegment>(entity);
        if (segment.grow)
        {
            AddSegment();
            segment.grow = false;
        }
    }
}
