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
#include "Worm.h"
#include "enemy.h"
#include "bullet.h"
#include "boost.h"
#include "machine.h"

using namespace dagger;
using namespace esccape;

namespace esccape
{
    entt::entity chrHealthBarEntity = entt::null;
    entt::entity skeletonHealthBarEntity = entt::null;
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
    engine.AddSystem<WormSystem>();
    engine.AddSystem<EsccapeControllerSystem>();
    engine.AddSystem<CharacterSystem>();
    engine.AddSystem<ShootingSystem>();
    engine.AddSystem<EnemySystem>();
    engine.AddSystem<MachineSystem>();
    engine.AddSystem<BoostSystem>();
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


void esccape::CreateHealthBar(int screenWidth, int screenHeight, int chrID)
{
    auto& reg = Engine::Registry();
    entt::entity healthBarEntity;

    if (chrHealthBarEntity == entt::null)
        chrHealthBarEntity = reg.create();
    if (skeletonHealthBarEntity == entt::null)
        skeletonHealthBarEntity = reg.create();

    if (chrID == 0) {
        healthBarEntity = chrHealthBarEntity;
    }
    else {
        healthBarEntity = skeletonHealthBarEntity;
    }
    auto& healthBarSprite = reg.emplace_or_replace<Sprite>(healthBarEntity);
    String hbName;

    AssignSprite(healthBarSprite, "Esccape:hb5");
    float ratio = healthBarSprite.size.y / healthBarSprite.size.x;
    float spriteSize = 150;
    healthBarSprite.size = { spriteSize, spriteSize * ratio };

    auto& transform = reg.emplace<Transform>(healthBarEntity);
    transform.position.x = -300;
    transform.position.y = screenHeight / 2 - 60;
    transform.position.z = 0;
    if (chrID == 1) {
        transform.position.x = 300;
    }

    // icon
    auto iconEntity = reg.create();
    auto& iconSprite = reg.emplace<Sprite>(iconEntity);
    if (chrID == 0)
        AssignSprite(iconSprite, "Esccape:chr_png");
    else
        AssignSprite(iconSprite, "Esccape:skeleton_png");

    float ratioIcon = iconSprite.size.y / iconSprite.size.x;
    float iconSpriteSize = 30;
    iconSprite.size = { iconSpriteSize, iconSpriteSize * ratioIcon };

    auto& transform1 = reg.emplace<Transform>(iconEntity);
    if (chrID == 0) {
        transform1.position.x = -200;
        transform1.position.y = screenHeight / 2 - 60;
        transform1.position.z = 0;
    }
    else {
        transform1.position.x = 200;
        transform1.position.y = screenHeight / 2 - 60;
        transform1.position.z = 0;
    }
    

}

void esccape::UpdateHealthBar(int chrID, int health)
{
    auto& reg = Engine::Registry();
    entt::entity healthBarEntity;
    if (chrID == 0)
        healthBarEntity = chrHealthBarEntity;
    else
        healthBarEntity = skeletonHealthBarEntity;

    auto& healthBarSprite = reg.get<Sprite>(healthBarEntity);
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
}

void esccape::onHealthChanged(const HealthChanged& event) {
    // health ide od 0 do 10
    // imamo srca : 0, 1, 2, 3, 4, 5
        //  health: 0, 2, 4, 6, 8, 10
    constexpr float tolerance = 0.01f;

    if (std::abs(event.newHealth - 8.0f) < tolerance)
        UpdateHealthBar(event.characterID, 4);
    else if (std::abs(event.newHealth - 6.0f) < tolerance)
        UpdateHealthBar(event.characterID, 3);
    else if (std::abs(event.newHealth - 4.0f) < tolerance)
        UpdateHealthBar(event.characterID, 2);
    else if (std::abs(event.newHealth - 2.0f) < tolerance)
        UpdateHealthBar(event.characterID, 1);
    else if (std::abs(event.newHealth) < tolerance)
        UpdateHealthBar(event.characterID, 0);
    else if (std::abs(event.newHealth - tolerance) >= 8.0)
        UpdateHealthBar(event.characterID, 5);
    else if (std::abs(event.newHealth - tolerance) >= 6.0)
        UpdateHealthBar(event.characterID, 4);
    else if (std::abs(event.newHealth - tolerance) >= 4.0)
        UpdateHealthBar(event.characterID, 3);
    else if (std::abs(event.newHealth - tolerance) >= 2.0)
        UpdateHealthBar(event.characterID, 2);
    else if (std::abs(event.newHealth - tolerance) >= 0.0)
        UpdateHealthBar(event.characterID, 1);
}


void esccape::CreateMachineRandom(int screenWidth, int screenHeight, int zPos, int machineScale)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();

    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "Esccape:masina");
    float ratio = sprite.size.y / sprite.size.x;    
    float machineSize = 250;              
    sprite.size = { machineSize, machineSize * ratio };

    auto& enemy = reg.emplace<Machine>(entity);
    
    auto& transform = reg.emplace<Transform>(entity); 

    transform.position.x = 250;
    transform.position.y = -200;
    transform.position.z = zPos;

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size.x = machineSize;
    col.size.y = machineSize * ratio;
}

void esccape::CreateEnemy(int zPos, int screenWidth, int screenHeight, Vector2 position) {
    auto& reg = Engine::Registry();
    auto entity = reg.create();

    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "Esccape:turret");
    float enemySize = 100;
    float ratio = sprite.size.y / sprite.size.x;
    sprite.size = { enemySize, enemySize * ratio };

    auto& enemy = reg.emplace<Enemy>(entity);

    auto& transform = reg.emplace<Transform>(entity);
    transform.position.x = position.x;
    transform.position.y = position.y;
    transform.position.z = zPos;

    auto& collision = reg.get_or_emplace<SimpleCollision>(entity);
    collision.size.x = enemySize;
    collision.size.y = enemySize * ratio;
}

void esccape::CreateObstacles(int zPos)
{
    int numberOfRocks = rand() % 4 + 1;
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

        int minY = -250, maxY = 150;
        int minX = -250, maxX = 250;
        
        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = rand() % (maxX - minX + 1) + minX;
        transform.position.y = rand() % (maxY - minY + 1) + minY;
        transform.position.z = zPos;

    }
}

void esccape::SetupWorld()
{
    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    reg.clear();
    Engine::Dispatcher().clear();


    constexpr Vector2 scale(1, 1);

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


    CreateMachineRandom(screenWidth, screenHeight, zPos, 3);
    CreateNWorms(4, zPos, screenWidth, screenHeight);
    CreateEnemy(zPos, screenWidth, screenHeight, { -300, 150 });
    CreateEnemy(zPos, screenWidth, screenHeight, { -300, -175 });


    CreateObstacles(zPos);

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
            auto entity = reg.create();
            Character mainChar(entity, "ASDWSpace",
                "spritesheets:player_anim:player_idle_front:1",
                "player:player_idle_front",
                { 1, 1, 1 }, { -300, 0 }, 0);
            mainChar.character->speed = 100.f;
            mainChar.character->inputContextReversed = false;
            reg.emplace<Character>(entity, std::move(mainChar));
         
        }
    }

    // skeleton character

    {
        {
            auto entity = reg.create();
            Character skeletonChar(entity, "skeleton-arrows",
                "spritesheets:skeleton:skeleton_idle_front:1",
                "skeleton:skeleton_idle_front",
                { 1, 1, 1 }, { 300, 0 }, 1);
            skeletonChar.character->speed = 100.f;
            skeletonChar.character->inputContextReversed = false;
            reg.emplace<Character>(entity, std::move(skeletonChar));
        }
    }

    
    CreateHealthBar(screenWidth, screenHeight, 0);
    CreateHealthBar(screenWidth, screenHeight, 1);
    Engine::Dispatcher().sink<HealthChanged>().connect<&onHealthChanged>();
   
}

