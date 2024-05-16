#include "deadend_main.h"
#include "deadend_bullet.h"
#include "deadend_camera.h"
#include "deadend_crosshair.h"
#include "deadend_player.h"
#include "deadend_playerinput.h"
#include "deadend_map.h"
#include "deadend_obstacle.h"

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
    //engine.AddPausableSystem<AimSystem>();
    engine.AddPausableSystem<PlayerSystem>();
    engine.AddPausableSystem<ShootingSystem>();
    engine.AddPausableSystem<DeadEndObstacleSystem>();
    
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

    constexpr float playerSize = 40.f;

    float zPos = 5.f;

    // generate map
    loadTiles(zPos);

    zPos -= 1.f;
    loadObstacles(zPos);
    //loadEnemies( zPos, playerSize);


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
        AssignSprite(sprite, "dead_end:Player:car");
        sprite.size.x = playerSize ;
        sprite.size.y = playerSize ;

        auto& player = reg.emplace<Player>(entity);
        auto& camera = reg.emplace<DeadEndCamera>(entity);
        auto& controller = reg.emplace<ControllerMapping>(entity);

        DeadEndPlayerInputSystem::SetupPlayerInput(controller);
    }

}
