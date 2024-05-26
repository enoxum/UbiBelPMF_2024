#include "deadend_main.h"
#include "deadend_bullet.h"
#include "deadend_camera.h"
#include "deadend_crosshair.h"
#include "deadend_player.h"
#include "deadend_playerinput.h"
#include "deadend_map.h"
#include "deadend_obstacle.h"
#include "deadend_health.h"
#include "deadend_hud.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"
#include "core/graphics/textures.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/animations.h"
#include "tools/diagnostics.h"
#include "core/graphics/gui.h"

#include "gameplay/common/simple_collisions.h"
#include "deadend_wavestruct.h"
#include "deadend_enemy.h"



using namespace dagger;
using namespace dead_end;


void dead_end::DeadEndGame::CoreSystemsSetup()
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

void dead_end::DeadEndGame::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddPausableSystem<SimpleCollisionsSystem>();
    engine.AddPausableSystem<DeadEndCameraFollowSystem>();
    engine.AddPausableSystem<DeadEndPlayerInputSystem>();
    engine.AddPausableSystem<AimSystem>();
    engine.AddPausableSystem<PlayerSystem>();
    engine.AddPausableSystem<ShootingSystem>();
    engine.AddPausableSystem<DeadEndObstacleSystem>();
    engine.AddPausableSystem<HealthSystem>();
   // engine.AddPausableSystem<DeadEndHUDSystem>();
    engine.AddPausableSystem<WaveSystem>();
    engine.AddPausableSystem< DeadEndEnemySystem>();
    
}

void dead_end::DeadEndGame::WorldSetup()
{
    //ShaderSystem::Use("standard");
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    dead_end::setupWorld();
}

void dead_end::setupWorld()
{
    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    // field
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;

    constexpr float playerSize = 90.f;

    float zPos = 5.f;

    // generate map
    loadTiles(zPos);

    zPos -= 1.f;
    loadObstacles(zPos);
    //loadEnemies(zPos-2.f, playerSize);


    // player
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = playerSize;
        col.size.y = playerSize;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 100;
        transform.position.y = 100;
        transform.position.z = zPos ;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "dead_end:Player:player_handgun");
        sprite.size.x = playerSize ;
        sprite.size.y = playerSize ;

        auto& player = reg.emplace<Player>(entity);
        auto& camera = reg.emplace<DeadEndCamera>(entity);
        auto& controller = reg.emplace<ControllerMapping>(entity);
        auto& health = reg.emplace<Health>(entity);
        health.currentHealth = 100;
        health.maxHealth = 100;

        DeadEndPlayerInputSystem::SetupPlayerInput(controller);
    }

    // crosshair
    {
        auto entity = reg.create();
        auto& transform = reg.emplace<Transform>(entity);

        auto& crosshair = reg.emplace<Aim>(entity);

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "dead_end:Crosshair:crosshair");
        sprite.size.x = playerSize / 2;
        sprite.size.y = playerSize / 2;
    }

    // HUD fill
    //{
    //    auto entity = reg.create();
    //    auto& hud = reg.emplace<HUD>(entity);
    //
    //    auto& transform = reg.emplace<Transform>(entity);
    //    
    //    transform.position.x = hud.position.x;
    //    transform.position.y = hud.position.y;
    //    transform.position.z = zPos - 2.f;
    //
    //    auto& fill_sprite = reg.emplace<Sprite>(entity);
    //
    //    AssignSprite(fill_sprite, "dead_end:Health:Health_1");
    //
    //}
    //
    // HUD border
    //{
    //   auto entity = reg.create();
    //    auto& hud = reg.emplace<HUD>(entity);
    //
    //    auto& transform = reg.emplace<Transform>(entity);
    //
    //    transform.position.x = hud.position.x;
    //    transform.position.y = hud.position.y;
    //    transform.position.z = zPos - 2.f;
    //
    //    auto& border_sprite = reg.emplace<Sprite>(entity);
    //
    //    AssignSprite(border_sprite, "dead_end:Health:Border_0");
    //}


    // waves
    {
        auto entity = reg.create();
        auto& wave = reg.emplace<Wave>(entity);

    }

}
