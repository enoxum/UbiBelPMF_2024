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

Text appleCounterText;
int count = 0;

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

    snakeTickCounter = 0;
    count = 0;
    UpdateCounter(count);
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
        //auto& lastSegment = reg.get<SnakeSegment>(snakeSegments.back());
        auto& lastTransform = reg.get<Transform>(snakeSegments.back());
        transform.position = lastTransform.position;
    }

    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "RedSnake:snake");
    sprite.color = ColorRGBA(0.5f, 1.0f, 0.5f, 1.0f);    
    sprite.size = Vector2(1, 1) * 20.0f;

    auto& snakeBody = reg.emplace<SnakeSegment>(entity);
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

        Vector3 distanceV = prevSegment.transform.position - segment.next_position;
        segment.next_position += distanceV / 2.f;

        //segment.next_position = prevSegment.transform.position;
    }

}

void red_snake::RedSnakeSystem::CreateSnake(float tileSize_, ColorRGBA color_, Vector3 pos_)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();

    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "RedSnake:head_left");
    sprite.size = Vector2(1, 1) * tileSize_;
    sprite.color = ColorRGBA(0.0f, 0.5f, 0.0f, 1.0f);
    sprite.size = Vector2(1, 1);

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

void red_snake::RedSnakeSystem::ResetGame()
{
    auto& reg = Engine::Registry();

    for (auto segment : snakeSegments)
    {
        reg.destroy(segment);
    }
    snakeSegments.clear();

    auto viewFood = reg.view<Food>();
    for (auto entity : viewFood)
    {
        reg.destroy(entity);
    }

    SpinUp();
}





void red_snake::RedSnakeSystem::UpdateCounter(int applesEaten) {
    appleCounterText.spacing = 0.7f;
    appleCounterText.Set("pixel-font", "Apples eaten: " + std::to_string(applesEaten), Vector3(10, 250, 0), true);
}


void RedSnakeSystem::Run()
{
    snakeTickCounter += Engine::DeltaTime();

    if (snakeTickCounter < snakeTick)
    {
        return;
    }
    else
    {
        snakeTickCounter = 0;
    }

    auto& reg = Engine::Registry();
    auto viewCollisions = reg.view<Transform, SimpleCollision>();
    auto view = reg.view<SnakeSegment, Transform, SimpleCollision>();
    const float segmentSpacing = 20.0f;
    std::vector<Vector3> previousPositions(snakeSegments.size());

    Vector3 previousSegmentPosition;

    bool test = false;

    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& col = view.get<SimpleCollision>(entity);

        if (reg.has<SnakeHead>(entity))
        {
            previousSegmentPosition = t.position;

            auto& segment = reg.get<SnakeSegment>(entity);

            //Move head
            /*auto& head = reg.get<SnakeHead>(entity);
            t.position += head.direction * snakeSpeed;*/

            auto& head = reg.get<SnakeHead>(entity);
            t.position += head.direction * segmentSpacing;

            // Promena sprite-a na osnovu smera kretanja
            auto& sprite = reg.get<Sprite>(entity);
            if (head.direction == Vector3(0, 1, 0))
            {
                AssignSprite(sprite, "RedSnake:head_up");
            }
            else if (head.direction == Vector3(0, -1, 0))
            {
                AssignSprite(sprite, "RedSnake:head_down");
            }
            else if (head.direction == Vector3(-1, 0, 0))
            {
                AssignSprite(sprite, "RedSnake:head_left");
            }
            else if (head.direction == Vector3(1, 0, 0))
            {
                AssignSprite(sprite, "RedSnake:head_right");
            }

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


        if (t.position.x >= (26 / 2 + 0.5f) * 20.f + 5 ||
            t.position.x <= -(26 / 2 + 0.5f) * 20.f - 5 ||
            t.position.y >= (20 / 2 + 0.5f) * 20.f + 5 ||
            t.position.y <= -(20 / 2 + 0.5f) * 20.f - 5
            ) {
            HWND hwnd = CreateWindow("myWindowClass", "Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                100, 100, 500, 400, NULL, NULL, NULL, NULL);
            int result = MessageBox(hwnd, "Game Over! Do you want to play again?", "RedSnake", MB_YESNO);

            if (result == IDYES)
            {
                ResetGame();
            }
            else if (result == IDNO)
            {
                PostQuitMessage(0);
            }
            return;
        }
    }


    auto& headTransform = reg.get<Transform>(snakeSegments[0]);
    for (size_t i = 1; i < snakeSegments.size(); ++i)
    {
        auto& segmentTransform = reg.get<Transform>(snakeSegments[i]);
        if (headTransform.position == segmentTransform.position)
        {
            // Kolizija detektovana, prekini igru
            HWND hwnd = CreateWindow("myWindowClass", "Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                100, 100, 500, 400, NULL, NULL, NULL, NULL);
            int result = MessageBox(hwnd, "Game Over! Do you want to play again?", "RedSnake", MB_YESNO);

            if (result == IDYES)
            {
                ResetGame();
            }
            else if (result == IDNO)
            {
                PostQuitMessage(0);
            }
            return;
        }
    }

    for (size_t i = snakeSegments.size() - 1; i > 0; --i)
    {
        auto& segment = reg.get<SnakeSegment>(snakeSegments[i]);
        auto& transform = reg.get<Transform>(snakeSegments[i]);

        Vector3 oldPosition = transform.position;
        transform.position = previousSegmentPosition;

        previousSegmentPosition = oldPosition;
    }


    for (auto entity : view)
    {
        auto& segment = view.get<SnakeSegment>(entity);
        if (segment.grow)
        {
            segment.grow = false;
            AddSegment();
            break;
        }
    }
}

