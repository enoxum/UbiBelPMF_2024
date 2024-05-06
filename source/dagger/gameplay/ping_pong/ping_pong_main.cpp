#include "ping_pong_main.h"

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
#include "gameplay/ping_pong/pingpong_ball.h"
#include "gameplay/ping_pong/player_scores.h"
#include "gameplay/ping_pong/pingpong_playerinput.h"
#include "gameplay/ping_pong/pingpong_tools.h"
#include "gameplay/ping_pong/bullethell_eye.h"
#include "gameplay/ping_pong/bullethell_bullet.h"
#include "gameplay/ping_pong/bullethell_bar.h"
#include "gameplay/ping_pong/bullethell_dash.h"


using namespace dagger;
using namespace ping_pong;

void ping_pong::CreatePingPongBall(float tileSize_, ColorRGBA color_, Vector3 speed_, Vector3 pos_)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "PingPong:ball");
    sprite.size = Vector2(1, 1) * tileSize_;

    sprite.color = color_;

    auto& transform = reg.emplace<Transform>(entity);
    transform.position = pos_ * tileSize_;
    transform.position.z = pos_.z;
    auto& ball = reg.emplace<PingPongBall>(entity);
    ball.speed = speed_ * tileSize_;

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size.x = tileSize_;
    col.size.y = tileSize_;

}

void PingPongGame::CoreSystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddSystem<WindowSystem>();
    engine.AddSystem<InputSystem>();
    engine.AddSystem<ShaderSystem>();
    engine.AddSystem<TextureSystem>();
    engine.AddSystem<SpriteRenderSystem>();
    engine.AddPausableSystem<TransformSystem>();
    engine.AddPausableSystem<AnimationSystem>();
    engine.AddSystem<DamageSystem>();
    engine.AddSystem<BulletSystem>();
    engine.AddSystem<BarSystem>();
    engine.AddSystem<DashSystem>();
#if !defined(NDEBUG)
    engine.AddSystem<DiagnosticSystem>();
    engine.AddSystem<GUISystem>();
    engine.AddSystem<ToolMenuSystem>();
#endif //!defined(NDEBUG)
}

void PingPongGame::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddPausableSystem<SimpleCollisionsSystem>();
    engine.AddPausableSystem<PingPongBallSystem>();
    engine.AddPausableSystem<PingPongPlayerInputSystem>();
    engine.AddPausableSystem<PlayerScoresSystem>();
    engine.AddPausableSystem<EyeSystem>();

#if defined(DAGGER_DEBUG)
    engine.AddPausableSystem<PingPongTools>();
#endif //defined(DAGGER_DEBUG)
}

void PingPongGame::WorldSetup()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    SetupWorld();
}

void ping_pong::SetupWorld()
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

//    float zPos = 1.f;

    constexpr float Space = 0.1f;

    float zPos = 0.f;



    // player controller setup
    const Float32 playerSize = tileSize * ((height - 2) * (1 + Space) * 0.33f);
    PingPongPlayerInputSystem::SetupPlayerBoarders(playerSize, -playerSize);
//    PingPongPlayerInputSystem::s_PlayerSpeed = tileSize * 14.f;
    //1st player
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = tileSize;
        col.size.y = tileSize;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 0;
        transform.position.y = 0;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "EmptyWhitePixel");
        sprite.size.x = tileSize;
        sprite.size.y = tileSize;
//        sprite.scale = {10, 1};
        reg.emplace<EyeTarget>(entity);

        auto& controller = reg.emplace<ControllerMapping>(entity);

        auto& mov = reg.emplace<MovementData>(entity);

        auto& dash = reg.emplace<DashData>(entity);
        dash.dashCooldown = 3.f;
        dash.dashTimer = 3.f;
        dash.canDash = true;

        createBar(&dash.dashTimer, {-250, -180, 0}, 200, 50, {0,0,1,1}, dash.dashCooldown);

        auto& stats = reg.emplace<StatsData>(entity);

        createBar(&stats.hp, {-250, -250, 0}, 200, 50, {1,0,0,1}, stats.hp);

        reg.emplace<Damageable>(entity);

        PingPongPlayerInputSystem::SetupPlayerOneInput(controller);
    }
    //Eye
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = tileSize;
        col.size.y = tileSize;

        reg.emplace<Eye>(entity);

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 0;
        transform.position.y = 0;
        transform.position.z = zPos;

        auto& stats = reg.emplace<StatsData>(entity);
        stats.hp = 1e2;

        createBar(&stats.hp, {0, 250, 0},400, 50, {1,0,0,1},stats.hp);

        auto& dmg = reg.emplace<Damageable>(entity);
        dmg.hurtDuration = .2f;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "bullet_hell:eye:IDLE:idle1");
        sprite.size.x = tileSize;
        sprite.size.y = tileSize;
        sprite.scale = {5, 5};

        auto &anim = reg.emplace<Animator>(entity);
        AnimatorPlay(anim, "bullet_hell:eye:IDLE");

    }


    // add score system to count scores for left and right collisions
    PlayerScoresSystem::SetFieldSize(width, height, tileSize * (1 + Space));
}


