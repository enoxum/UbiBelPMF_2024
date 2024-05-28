#pragma once
#include <vector>
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

namespace red_snake
{
    struct SnakeSegment
    {
        Transform transform;
        Sprite sprite;
        Vector3 speed{ 0, 0, 0 };
        Vector3 next_position{};
        Vector3 direction{ 0, 0, 0 };


        bool grow{ false };
        bool alive{ true };
    };

    struct Food
    {
        bool eat{ false };
    };

    struct SnakeHead : public SnakeSegment
    {
    };

    struct SnakeBody : public SnakeSegment
    {
    };

    class RedSnakeSystem : public System
    {
    public:
        float snakeTick = 0.15f;
        float snakeTickCounter = 0;

        float snakeSpeed = 20;

    public:
        inline String SystemName() { return "Red Snake System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;
        void AddSegment();
        void UpdateSegmentsPositions();
        void CreateSnake(float tileSize_, ColorRGBA color_, Vector3 pos_);
        //void CreateFood(float tileSize_, ColorRGBA color_, Vector3 pos_);
        std::vector<entt::entity> snakeSegments;
        void ResetGame();
        void UpdateCounter(int applesEaten);
    };
}
