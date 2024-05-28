#include "snake.h"
#include "snake_playerinput.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "core/graphics/text.h"
#include "gameplay/common/simple_collisions.h"
#include <iostream>
#include "red_snake_main.h"

using namespace dagger;
using namespace red_snake;

void RedSnakeSystem::SpinUp()
{
    constexpr int height = 20;
    constexpr int width = 26;
    constexpr float tileSize = 20.f;
    float zPos = 1.f;
    snakeSegments.clear();
    CreateSnake(tileSize, ColorRGBA(0, 1, 0, 1), { width / 2, height / 2, zPos });
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int foodX = (std::rand()) % 24 + 1;
    int foodY = (std::rand()) % 18 + 1;
    CreateFood(tileSize, ColorRGBA(1, 0, 0, 1), { foodX, foodY, zPos });

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
        transform.position = lastTransform.position; 
    }

    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "RedSnake:snake");
    sprite.size = Vector2(1, 1) * 20.0f;

    auto& snakeBody = reg.emplace<SnakeBody>(entity);
    snakeSegments.push_back(entity);
    /*auto& controller = reg.emplace<SnakeControllerMapping>(entity);
    SnakePlayerInputSystem::SetupPlayerInput(controller);*/
}

void RedSnakeSystem::UpdateSegmentsPositions()
{
    auto& reg = Engine::Registry();

    auto& headSegment = reg.get<SnakeSegment>(snakeSegments[0]);
    headSegment.next_position = headSegment.transform.position + headSegment.direction * 20.0f;
    for (size_t i = snakeSegments.size() - 1; i > 0; --i)
    {
        auto& segment = reg.get<SnakeSegment>(snakeSegments[i]);
        auto& prevSegment = reg.get<SnakeSegment>(snakeSegments[i - 1]);
        segment.next_position = prevSegment.transform.position;
    }

}

void red_snake::RedSnakeSystem::CreateSnake(float tileSize_, ColorRGBA color_, Vector3 pos_)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();

    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "RedSnake:snake");
    sprite.size = Vector2(1, 1) * tileSize_;
    //sprite.color = color_;

    auto& transform = reg.emplace<Transform>(entity);

    transform.position.x = (pos_.x + 0.5f) * tileSize_;
    transform.position.y = (pos_.y + 0.5f) * tileSize_;
    transform.position.z = pos_.z;

    auto& snake = reg.emplace<SnakeSegment>(entity);
    snake.alive = true;

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size.x = tileSize_;
    col.size.y = tileSize_;

    reg.emplace<SnakeHead>(entity);

    auto& controller = reg.emplace<SnakeControllerMapping>(entity);
    SnakePlayerInputSystem::SetupPlayerInput(controller);
    snakeSegments.push_back(entity);
}



Text appleCounterText;
int count = 0;

void UpdateCounter(int applesEaten) {
    appleCounterText.spacing = 0.7f;
    appleCounterText.Set("pixel-font", "Apples eaten: " + std::to_string(applesEaten), Vector3(10, 250, 0), true);
}


void RedSnakeSystem::Run()
{
    auto& reg = Engine::Registry();
    auto viewCollisions = reg.view<Transform, SimpleCollision>();
    auto view = reg.view<SnakeSegment, Transform, SimpleCollision>();

    const float segmentSpacing = 20.0f; 
    std::vector<Vector3> previousPositions(snakeSegments.size());


    for (size_t i = 0; i < snakeSegments.size(); ++i)
    {
        auto& segmentTransform = reg.get<Transform>(snakeSegments[i]);
        previousPositions[i] = segmentTransform.position;
    }

    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& segment = view.get<SnakeSegment>(entity);
        auto& col = view.get<SimpleCollision>(entity);

        if (reg.has<SnakeHead>(entity))
        {
            Vector3 direction = segment.direction * segmentSpacing;
            t.position += direction;

            if (col.colided)
            {
                if (reg.valid(col.colidedWith))
                {
                    SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
                    Transform& transform = viewCollisions.get<Transform>(col.colidedWith);

                    if (reg.has<Food>(col.colidedWith))
                    {
                        segment.grow = true;
                        reg.destroy(col.colidedWith);
                        int foodX = std::rand() % 24 + 1;
                        int foodY = std::rand() % 18 + 1;
                        CreateFood(segmentSpacing, ColorRGBA(1, 0, 0, 1), { foodX, foodY, 1.0f });
                        count++;
                        UpdateCounter(count);
                    }
                    else if (reg.has<SnakeSegment>(col.colidedWith))
                    {
                        segment.alive = false;
                    }

                    col.colided = false;
                }
            }
        }
    }

    for (size_t i = snakeSegments.size() - 1; i > 0; --i)
    {
        auto& segmentTransform = reg.get<Transform>(snakeSegments[i]);
        segmentTransform.position = previousPositions[i - 1];
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

