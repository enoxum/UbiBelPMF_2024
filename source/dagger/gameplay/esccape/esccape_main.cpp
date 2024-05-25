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
#include "esccape_controller.h"
#include "character_system.h"
#include <random>
#include <cstdlib>
#include "blackboard_manager.h"

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
    engine.AddSystem<EsccapeControllerSystem>();
    engine.AddSystem<CharacterSystem>();
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
    // Fix
    auto& healthBarReg = Engine::Registry();

    /*if (healthBarEntity != entt::null) {
        printf("aaaaaa");
        healthBarReg.destroy(healthBarEntity);
    }*/
    
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


void esccape::CreateMachineRandom(int screenWidth, int screenHeight, int zPos, int machineScale)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "Esccape:masina");
    float ratio = sprite.size.y / sprite.size.x;    
    float machineSize = 300;              
    sprite.size = { machineSize, machineSize * ratio };
    
    auto& transform = reg.emplace<Transform>(entity); 

    transform.position.x = 250;
    transform.position.y = -200;
    transform.position.z = zPos;

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size.x = machineSize;
    col.size.y = machineSize * ratio;
}

void esccape::CreateObstacles(int zPos)
{
    int numberOfRocks = rand() % 10 + 3;
    printf("%d", numberOfRocks);

    for (int i = 0; i < numberOfRocks; i++)
    {
        int rockSize = rand() % 50 + 50;
        auto& reg = Engine::Registry();
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "Esccape:rock2");
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { rockSize, rockSize * ratio };

        auto& transform = reg.emplace<Transform>(entity);

        int minY = -300, maxY = 150;
        int minX = -350, maxX = 180;
        transform.position.x = rand() % (maxX - minX + 1) + minX;
        transform.position.y = rand() % (maxY - minY + 1) + minY;
        transform.position.z = zPos;

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = rockSize;
        col.size.y = rockSize * ratio;
    }
}

//// CreatingEnemy
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

    
    CreateHealthBar(screenWidth, screenHeight, zPos, 5);
    CreateMachineRandom(screenWidth, screenHeight, zPos, 3);
    //CreateObstacles(zPos);

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
            transform.position.y = screenHeight / 2 - 50;
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
            transform.position.y = -screenHeight / 2 + 50;
            transform.position.z = zPos;
        }

        //left
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = 0;
            col.size.y = screenHeight;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = -screenWidth / 2 + 30;
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
            transform.position.x = screenWidth / 2 - 30;
            transform.position.y = 0;
            transform.position.z = zPos;
        }
    }


    // main character
    {
        {
            auto mainChar = Character::Create("ASDWSpace",
                "spritesheets:player_anim:player_idle_front:1",
                "player:player_idle_front",
                { 1, 1, 1 }, { 0, 100 }, 0);

            auto entity = mainChar.getEntity();
            auto& character = reg.emplace<Character>(entity, mainChar);
        }
    }

    // skeleton character
    {
        {
            auto skeletonChar = Character::Create("skeleton-arrows",
                "spritesheets:skeleton:skeleton_idle_front:1",
                "skeleton:skeleton_idle_front",
                { 1, 1, 1 }, { -100, 0 }, 1);

            auto entity = skeletonChar.getEntity();
            auto& character = reg.emplace<Character>(entity, skeletonChar);
        }
    }
    
}

