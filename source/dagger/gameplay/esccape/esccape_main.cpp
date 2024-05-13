#include "esccape_main.h"

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
#include "tools/diagnostics.h"

#include "gameplay/common/simple_collisions.h"

#include "esccape_main.h"
#include "player.h"

#include <random>
#include <cstdlib>

using namespace dagger;
using namespace esccape;

void EsccapeGame::CoreSystemsSetup()
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


void EsccapeGame::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddPausableSystem<SimpleCollisionsSystem>();
    engine.AddSystem<Player>();
}

void EsccapeGame::WorldSetup()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    SetupWorld();
}

void esccape::CreateMachineRandom(float playerSize, int screenWidth, int screenHeight, int zPos, int machineScale)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "Esccape:masina2");
    float ratio = sprite.size.y / sprite.size.x;    
    float machineSize = playerSize * machineScale;
    printf("%f",machineSize);
    sprite.size = { machineSize, machineSize * ratio };

    auto& transform = reg.emplace<Transform>(entity);
    transform.position.x = rand() % (screenWidth - (int)machineSize) - screenWidth/2;
    transform.position.y = rand() % (screenHeight - (int)machineSize) - screenHeight/2;
    transform.position.z = zPos;

    if (transform.position.x <= playerSize && transform.position.x >= -playerSize)
        transform.position.x += 3 * playerSize;
    if (transform.position.y <= playerSize && transform.position.y >= -playerSize)
        transform.position.y += 3 * playerSize;
    

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size.x = machineSize;
    col.size.y = machineSize;
}


void esccape::SetupWorld()
{
    Vector2 scale(1, 1);

    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    // field
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;

    constexpr float playerSize = 80.f;// / static_cast<float>(Width);

    float zPos = 1.f;

    
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "Esccape:pesak");
        sprite.size = { screenWidth, screenHeight};

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 0, 0, zPos };
    }


    zPos -= 1.f;

    // create machine
    CreateMachineRandom(playerSize, screenWidth, screenHeight, zPos, 3);


    // collisions
    {
        // up
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = screenWidth;
            col.size.y = playerSize;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = 0;
            transform.position.y = screenHeight / 2;
            transform.position.z = zPos;
        }

        // down
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = screenWidth;
            col.size.y = playerSize;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = 0;
            transform.position.y = -screenHeight / 2;
            transform.position.z = zPos;
        }

        // TODO - left & right
    }




    // player
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "Esccape:djura");
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { playerSize, playerSize * ratio };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = {0, 0, zPos };

        auto& racingPlayer = reg.emplace<PlayerEntity>(entity);
        racingPlayer.speed = playerSize * 3;  


        reg.emplace<ControllerMapping>(entity);

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = sprite.size;
    }

}

