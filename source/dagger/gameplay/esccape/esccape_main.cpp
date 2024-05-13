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

void esccape::SetupWorld()
{
    Vector2 scale(1, 1);

    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    // field
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;

    constexpr int height = 20;
    constexpr int width = 26;
    constexpr float tileSize = 20.f;// / static_cast<float>(Width);

    float zPos = 1.f;

    zPos -= 1.f;

    /*   for (int i = 1; i < 7; i++)
       {

               auto entity = reg.create();
               auto& sprite = reg.emplace<Sprite>(entity);
               AssignSprite(sprite, fmt::format("spritesheets:player:player_idle_front_{}", i));
               sprite.position = { i * 48,48, 99 };

       }*/


        for (int i = 0; i < 10; i++)
        {       
            auto player = reg.create();
            auto& sprite = reg.emplace<Sprite>(player);
            AssignSprite(sprite, "spritesheets:player_anim:player_idle_front:1");
            sprite.position = { rand() % 300 - 150, rand() % 300 - 150, 0 };
            sprite.position.z = (150.0f + sprite.position.y) / 10.0f;
            sprite.scale = { 3, 3 };

            auto& anim = reg.emplace<Animator>(player);
            AnimatorPlay(anim, "dungeon:player_idle_front");
        }

       // player
       /*{
           auto entity = reg.create();
           auto& sprite = reg.emplace<Sprite>(entity);
           AssignSprite(sprite, "Esccape:djura");
           float ratio = sprite.size.y / sprite.size.x;
           sprite.size = { 4 * tileSize, 4 * tileSize * ratio };

           auto& transform = reg.emplace<Transform>(entity);
           transform.position = { -tileSize * 4, -tileSize * 4, zPos };

           auto& racingPlayer = reg.emplace<PlayerEntity>(entity);
           racingPlayer.speed = tileSize * 6;

           reg.emplace<ControllerMapping>(entity);

           auto& col = reg.emplace<SimpleCollision>(entity);
           col.size = sprite.size;
       }*/
}
