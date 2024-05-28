#include "red_snake_main.h"
#include "snake.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "core/graphics/text.h"
#include "tools/diagnostics.h"

#include "gameplay/common/simple_collisions.h"
#include "snake_playerinput.h"
#include <iostream>


#include<random>

using namespace red_snake;
float RandomFloat(float min, float max);
//Text appleCounterText;

void red_snake::RedSnakeGame::CoreSystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddSystem<WindowSystem>();
    engine.AddSystem<InputSystem>();
    engine.AddSystem<ShaderSystem>();
    engine.AddSystem<TextureSystem>();
    engine.AddSystem<SpriteRenderSystem>();
    engine.AddPausableSystem<TransformSystem>();
    engine.AddPausableSystem<AnimationSystem>();
#if !defined(NDEBUG)
    engine.AddSystem<DiagnosticSystem>();
    engine.AddSystem<GUISystem>();
    engine.AddSystem<ToolMenuSystem>();
#endif //!defined(NDEBUG)
}

void red_snake::RedSnakeGame::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddPausableSystem<SimpleCollisionsSystem>();
    engine.AddPausableSystem<RedSnakeSystem>();
    engine.AddPausableSystem<SnakePlayerInputSystem>();
}

void red_snake::RedSnakeGame::WorldSetup()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    SetUpWorld();
}



void red_snake::CreateFood(float tileSize_, ColorRGBA color_, Vector3 pos_)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();

    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "RedSnake:food");
    sprite.size = Vector2(1, 1) * tileSize_;
    sprite.color = color_;

    auto& transform = reg.emplace<Transform>(entity);
    auto* camera = Engine::GetDefaultResource<Camera>();
    float gameWidth = camera->size.x;
    float gameHeight= camera->size.y;
    
    // mora ovo pametnije al neka stoji ovako za sad


    float randomX = RandomFloat(-(26 / 2 + 0.5f) * 20.f, (26 / 2 + 0.5f) * 20.f );
    float randomY = RandomFloat(-(20 / 2 + 0.5f) * 20.f, (20 / 2 + 0.5f) * 20.f );

    transform.position.x = randomX;
    transform.position.y = randomY;
    transform.position.z = pos_.z;

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size.x = tileSize_;
    col.size.y = tileSize_;

    reg.emplace<Food>(entity);
}

void red_snake::SetUpWorld()
{
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;
    constexpr int height = 20;
    constexpr int width = 26;
    constexpr float tileSize = 20.f;
    constexpr float Space = 0.1f;

    auto& reg = Engine::Registry();
    float zPos = 1.f;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, "EmptyWhitePixel");
            sprite.size = Vector2(1, 1) * tileSize;

            if (i % 2 != j % 2)
            {
                sprite.color.r = 0.4f;
                sprite.color.g = 0.4f;
                sprite.color.b = 0.4f;
            }
            else
            {
                sprite.color.r = 0.6f;
                sprite.color.g = 0.6f;
                sprite.color.b = 0.6f;
            }

            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
            {
                sprite.color.r = 0.0f;
                sprite.color.g = 0.0f;
                sprite.color.b = 0.0f;
            }

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f + j + j * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
            transform.position.y = (0.5f + i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
            transform.position.z = zPos;
        }
    }
    /*
    std::cout << "Creating snake..." << std::endl;
    CreateSnake(tileSize, ColorRGBA(0, 1, 0, 1), { width / 2, height / 2, zPos });

    std::cout << "Creating food..." << std::endl;
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int foodX = (std::rand()) % 24 + 1;
    int foodY = (std::rand()) % 18 + 1;
    CreateFood(tileSize, ColorRGBA(1, 0, 0, 1), { foodX, foodY, zPos });
    */
   // appleCounterText.spacing = 0.7f;
    //appleCounterText.Set("pixel-font", "Apples eaten: 0", Vector3(10, 250, 10), true);
   /*auto ui = reg.create();
    auto& text = reg.emplace<Text>(ui);
    text.spacing = 0.6f;
    text.Set("pixel-font", "hello world", Vector3(10, 250, 0), true);*/ 
}
float RandomFloat(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}