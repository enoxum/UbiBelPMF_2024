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

void esccape::CreateEnemy(int zPos, int screenWidth, int screenHeight) {
    auto& reg = Engine::Registry();
    auto entity = reg.create();

    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "Esccape:turret");
    float enemySize = 100;
    float ratio = sprite.size.y / sprite.size.x;
    sprite.size = { enemySize, enemySize * ratio };

    auto& enemy = reg.emplace<Enemy>(entity);

    auto& transform = reg.emplace<Transform>(entity);
    transform.position.x = -300;
    transform.position.y = 150;
    transform.position.z = zPos;

    auto& collision = reg.get_or_emplace<SimpleCollision>(entity);
    collision.size.x = enemySize;
    collision.size.y = enemySize * ratio;
}

void esccape::CreateObstacles(int zPos)
{
    int numberOfRocks = rand() % 2 + 1;
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

void esccape::CreateNWorms(int n, int zPos, int screenWidth, int screenHeight) {
    for (int i = 0; i < n; i++)
        CreateWorm(zPos, screenWidth, screenHeight);
}

void esccape::CreateWorm(int zPos, int screenWidth, int screenHeight) {
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "Esccape:crv");
    int wormSize = 60;
    float ratio = sprite.size.y / sprite.size.x;
    sprite.size = { wormSize, wormSize * ratio };

    auto& worm = reg.emplace<Worm>(entity);

    auto& transform = reg.emplace<Transform>(entity);

    worm.startingYTop = screenHeight / 2 + 50;
    worm.startingXRight = screenWidth / 2 + 30;
    worm.startingYBottom = -(screenHeight / 2 + 50);
    worm.startingXLeft = -(screenWidth / 2 + 30);
    worm.minimumX = -350; // 30
    worm.maximumX = 350;
    worm.minimumY = -250;
    worm.maximumY = 250;

    worm.scaleX = sprite.scale.x;
    worm.scaleY = sprite.scale.y;
    SpawnWorm(worm, transform, sprite);
}

void esccape::SpawnWorm(Worm& worm, Transform& t, Sprite& sprite) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disSpeed(200, 400);
    std::uniform_int_distribution<> disX(worm.minimumX, worm.maximumX);
    std::uniform_int_distribution<> disY(worm.minimumY, worm.maximumY);
    std::uniform_int_distribution<> disDistance(300, 1500);
    int edge = rand() % 4;

    int speed = disSpeed(gen);
    
    switch (edge) {
        case 0:
            t.position.x = disX(gen);
            t.position.y = worm.startingYTop + disDistance(gen);
            sprite.rotation = 0;
            worm.speed = { 0, -speed, 0 };
            break;
        case 1:
            t.position.x = worm.startingXRight + disDistance(gen);
            t.position.y = disY(gen);
            sprite.rotation = 270;
            worm.speed = { -speed, 0, 0 };
            break;
        case 2:
            t.position.x = disX(gen);
            t.position.y = worm.startingYBottom - disDistance(gen);
            sprite.rotation = 180;
            worm.speed = { 0, speed, 0 };
            break;
        case 3:
            t.position.x = worm.startingXLeft - disDistance(gen);
            t.position.y = disY(gen);
            sprite.rotation = 90;
            worm.speed = { speed, 0, 0 };
            break;
    }
}

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

    
    CreateMachineRandom(screenWidth, screenHeight, zPos, 3);
    CreateNWorms(4, zPos, screenWidth, screenHeight);
    CreateEnemy(zPos, screenWidth, screenHeight);

    CreateBullet(Vector2{ 0,0 }, Vector2{1, 0});

    //CreateWorm(zPos, screenWidth, screenHeight);
    //CreateWorm(zPos, screenWidth, screenHeight);
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
            //auto& character = reg.emplace<Character>(entity, mainChar);
            auto& character = reg.emplace<Character>(entity, std::move(mainChar));
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
            //printf("Character %d health = %f healthEnt = %f\n", character.character->id, character.character->health, character.character->healthSystem.GetCurrentHealth());
        }
    }
    
    CreateHealthBar(screenWidth, screenHeight, 0);
    CreateHealthBar(screenWidth, screenHeight, 1);
    Engine::Dispatcher().sink<HealthChanged>().connect<&onHealthChanged>();
   
}

