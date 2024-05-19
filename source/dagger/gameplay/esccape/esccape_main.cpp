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

namespace esccape
{
    entt::entity healthBarEntity = entt::null;
}

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


void esccape::CreateHealthBar(int screenWidth, int screenHeight, int zPos, int health)
{
    auto& healthBarReg = Engine::Registry();

    if (healthBarEntity != entt::null) {
        printf("aaaaaa");
        healthBarReg.destroy(healthBarEntity);
    }
    
    // Create a new health bar entity
    healthBarEntity = healthBarReg.create();
    auto& healthBarSprite = healthBarReg.emplace<Sprite>(healthBarEntity);
    String hbName;
    
    switch (health) {
    case 5:
        hbName = "Esccape:hb5";
        break;
    case 4:
        hbName = "Esccape:hb4";
        break;
    case 3:
        hbName = "Esccape:hb3";
        break;
    case 2:
        hbName = "Esccape:hb2";
        break;
    case 1:
        hbName = "Esccape:hb1";
        break;
    default:
        hbName = "Esccape:hb0";
    }

    AssignSprite(healthBarSprite, hbName);
    float ratio = healthBarSprite.size.y / healthBarSprite.size.x;
    float spriteSize = 150;
    healthBarSprite.size = { spriteSize, spriteSize * ratio };
    auto& transform = healthBarReg.emplace<Transform>(healthBarEntity);
    transform.position.x = -screenWidth/2 + 100;
    transform.position.y = screenHeight/2 - 60;
    transform.position.z = zPos;
}

void esccape::onHealthChanged(int newHealth)
{
    if (newHealth > 5)
        newHealth = 5;
    if (newHealth < 0)
        newHealth = 0;

    CreateHealthBar(800, 600, -1, newHealth);   
}


void esccape::CreateMachineRandom(float playerSize, int screenWidth, int screenHeight, int zPos, int machineScale)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "Esccape:masina");
    float ratio = sprite.size.y / sprite.size.x;    
    float machineSize = playerSize * machineScale;
    sprite.size = { machineSize, machineSize * ratio };
    
    auto& transform = reg.emplace<Transform>(entity); 

    int minY = -200, maxY = 100;
    int minX = -300, maxX = 300;
    transform.position.x = rand() % (maxX - minX + 1) + minX;
    transform.position.y = rand() % (maxY - minY + 1) + minY;
    transform.position.z = zPos;

    while ((transform.position.x <= playerSize && transform.position.x >= -playerSize) || (transform.position.y <= playerSize && transform.position.y >= -playerSize))
    {
        transform.position.x = rand() % (maxX - minX + 1) + minX;
        transform.position.y = rand() % (maxY - minY + 1) + minY;
    }

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size.x = machineSize;
    col.size.y = machineSize * ratio;
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

        //ATTACH_TO_FSM(CharacterControllerFSM, entity);

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

// CreatingEnemy
struct EnemyCharachter
{
    Entity entity;
    Sprite& sprite;
    EsccapeCharacter& character;

    static EnemyCharachter Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& character = reg.get_or_emplace<EsccapeCharacter>(entity);

        return EnemyCharachter{ entity, sprite, character };
    }

    static EnemyCharachter Create(
        ColorRGB color_ = { 1, 1, 1 },
        Vector2 position_ = { 0, 0 })
    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();

        auto chr = EnemyCharachter::Get(entity);

        chr.sprite.scale = { 0.2, 0.2 };
        chr.sprite.position = { position_, 0.0f };
        chr.sprite.color = { color_, 1.0f };

        AssignSprite(chr.sprite, "Esccape:turret");

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

    constexpr float playerSize = 80.f;// / static_cast<float>(Width);

    float zPos = 1.f;

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "Esccape:pesak");
        sprite.size = { screenWidth, screenHeight };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 0, 0, zPos };
    }

    zPos -= 1.f;

    // ui
    
    // health bar
    CreateHealthBar(screenWidth, screenHeight, zPos, 5);

    // create machine
    CreateMachineRandom(playerSize, screenWidth, screenHeight, zPos, 3);

    // collisions
    {
        // up
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = screenWidth;
            col.size.y = 0;

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
            col.size.y = 0;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = 0;
            transform.position.y = -screenHeight / 2;
            transform.position.z = zPos;
        }

        //left
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = 0;
            col.size.y = screenHeight;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = -screenWidth / 2;
            transform.position.y = 0;
            transform.position.z = zPos;
        }
        //right
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = 0;
            col.size.y = screenHeight;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = screenWidth / 2;
            transform.position.y = 0;
            transform.position.z = zPos;
        }
    }

    // player
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "Esccape:djura");
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { playerSize, playerSize * ratio };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 0, 0, zPos };

        auto& racingPlayer = reg.emplace<PlayerEntity>(entity);
        racingPlayer.speed = playerSize * 3;
        racingPlayer.health = 5;

        Player player = Player(racingPlayer, onHealthChanged);


        auto mainChar = Character::Create({ 1, 1, 1 }, { -100, 0 });

        auto enemyChar = EnemyCharachter::Create({ 1, 1, 1 }, { -100, 0 });

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

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size = sprite.size;
}

       auto& transform = reg.emplace<Transform>(player);
       transform.position = { -tileSize * 4, -tileSize * 4, zPos };

       auto& racingPlayer = reg.emplace<PlayerEntity>(player);
       racingPlayer.speed = tileSize * 6;

       reg.emplace<ControllerMapping>(player);
   */
    }
}

