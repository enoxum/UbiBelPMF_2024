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
        std::vector<entt::entity> snakeSegments;

    public:
        inline String SystemName() { return "Red Snake System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;
        void AddSegment();
        //void CreateSnake(float tileSize_, Vector3 pos_);
        //void CreateFood(float tileSize_, ColorRGBA color_, Vector3 pos_);
    };
}
