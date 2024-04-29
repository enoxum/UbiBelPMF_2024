#include "gameplay/bober_kuvar/bober_kuvar_main.h"

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

#include "gameplay/bober_kuvar/bober_kuvar_controller.h"
#include "gameplay/common/parallax.h"
#include "gameplay/common/camera_focus.h"

using namespace dagger;
using namespace bober_kuvar;
using namespace common;

void BoberKuvar::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddSystem<RoguelikeControllerSystem>();
    engine.AddSystem<CameraFollowSystem>();
}

void BoberKuvar::SetCamera()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 2;
    camera->position = { 0, 0, 0 };
    camera->Update();
}

void BoberKuvar::WorldSetup()
{
    //Engine::GetDefaultResource<Audio>()->PlayLoop("music");

    // bober
    auto bober = Engine::Instance().Registry().create();

    auto& transform = Engine::Instance().Registry().emplace<Transform>(bober);
    transform.position = Vector3{ 0.0f, 0.0f, 0.0f };

    auto& sprite = Engine::Instance().Registry().emplace<Sprite>(bober);
    AssignSprite(sprite, "souls_like_knight_character:IDLE:idle1");

    auto& anim = Engine::Instance().Registry().emplace<Animator>(bober);
    AnimatorPlay(anim, "souls_like_knight_character:IDLE");

    auto& controller = Engine::Instance().Registry().emplace<ControllerMapping>(bober);
    RoguelikeControllerSystem::SetupPlayerInput(controller);

    Engine::Registry().emplace<CameraFollowFocus>(bober);

    //enemy
    auto enemy = Engine::Instance().Registry().create();

    auto& transform1 = Engine::Instance().Registry().emplace<Transform>(enemy);
    transform1.position = Vector3{ 100.0f, 0.0f, 0.0f };

    auto& sprite1 = Engine::Instance().Registry().emplace<Sprite>(enemy);
    AssignSprite(sprite1, "souls_like_knight_character:IDLE:idle1");

    auto& anim1 = Engine::Instance().Registry().emplace<Animator>(enemy);
    AnimatorPlay(anim1, "souls_like_knight_character:IDLE");

    SetCamera();
}