#include "gameplay/bober_kuvar/BoberGame.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/audio.h"
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

#include "gameplay/common/parallax.h"
#include "gameplay/common/camera_focus.h"
#include "gameplay/common/aiming_system.h"

#include "Player.h"
#include "Enemy.h"
#include "PlayerController.h"
#include "Cursor.h"

using namespace dagger;
using namespace bober_game;
using namespace common;

void BoberGame::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddSystem<PlayerController>();
    engine.AddSystem<CameraFollowSystem>();
    //engine.AddSystem<AimingSystem>();
}

void BoberGame::SetCamera()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 2;
    camera->position = { 0, 0, 0 };
    camera->Update();
}

void BoberGame::WorldSetup()
{
    //Engine::GetDefaultResource<Audio>()->PlayLoop("music");
    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();
    // bober
    Player *bober = new Player();

    // enemy
    Enemy* enemy = new Enemy();
    enemy->move(Vector3{ 100.0f, 0.0f, 0.0f });

    OurEntity* cursor = new OurEntity();
    //Cursor
    {
        Vector2 scale(1, 1);
        float zPos = 1.f;
        constexpr float tileSize = 20.f;
        (*cursor->transform).position.x = 0;
        (*cursor->transform).position.y = 0;
        (*cursor->transform).position.z = zPos;

        AssignSprite(*cursor->sprite, "crosshair");
        (*cursor->sprite).size = scale * tileSize;
        reg.remove<Animator>(cursor->instance);
        reg.emplace<Cursor>(cursor->instance);
    }
    SetCamera();
}