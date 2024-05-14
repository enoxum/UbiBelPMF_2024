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
#include "character_controller_fsm.h"
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

//Creating player
struct Character
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    InputReceiver& input;
    EsccapeCharacter& character;

    static Character Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& character = reg.get_or_emplace<EsccapeCharacter>(entity);

        return Character{ entity, sprite, anim, input, character };
    }

    static Character Create(
        ColorRGB color_ = { 1, 1, 1 },
        Vector2 position_ = { 0, 0 })
    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();

        ATTACH_TO_FSM(CharacterControllerFSM, entity);

        auto chr = Character::Get(entity);

        chr.sprite.scale = { 2.5, 2.5 };
        chr.sprite.position = { position_, 0.0f };
        chr.sprite.color = { color_, 1.0f };

        AssignSprite(chr.sprite, "spritesheets:player_anim:player_idle_front:1");
        AnimatorPlay(chr.animator, "player:player_idle_front");

        chr.character.speed = 50;

        return chr;
    }
};


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

    auto mainChar = Character::Create({ 1, 1, 1 }, { -100, 0 });

            /*auto player = reg.create();

            auto& sprite = reg.emplace<Sprite>(player);
            AssignSprite(sprite, "spritesheets:player_anim:player_attack_right:1");
            sprite.position = { 0, 0 , 0};
            sprite.position.z = (150.0f + sprite.position.y) / 10.0f;
            sprite.scale = { 3, 3 };

            auto& anim = reg.emplace<Animator>(player);
            AnimatorPlay(anim, "player:player_attack_right");
           
           float ratio = sprite.size.y / sprite.size.x;
           sprite.size = { 4 * tileSize, 4 * tileSize * ratio };

           auto& transform = reg.emplace<Transform>(player);
           transform.position = { -tileSize * 4, -tileSize * 4, zPos };

           auto& racingPlayer = reg.emplace<PlayerEntity>(player);
           racingPlayer.speed = tileSize * 6;

           reg.emplace<ControllerMapping>(player);
       */
}
