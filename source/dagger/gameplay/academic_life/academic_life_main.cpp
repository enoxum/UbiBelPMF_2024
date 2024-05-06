#include "academic_life_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/common/particles.h"

#include "gameplay/academic_life/academic_life_logic.h"
#include "gameplay/academic_life/academic_player.h"
#include "gameplay/academic_life/falling_entity.h"

using namespace dagger;
using namespace academic_life;

void AcademicLife::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddSystem<AcademicPlayerInputSystem>();
    engine.AddSystem<FallingEntitySystem>();
    engine.AddSystem<AcademicLifeCollisionsLogicSystem>();
    engine.AddSystem<SimpleCollisionsSystem>();
    engine.AddSystem<common_res::ParticleSystem>();
}

void AcademicLife::WorldSetup()
{
    auto& engine = Engine::Instance();

    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 600, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    academic_life::SetupWorld();
}

void academic_life::SetupWorld()
{
    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    constexpr Vector2 scale(1, 1);

    constexpr int Heigh = 30;
    constexpr int Width = 21;
    constexpr float TileSize = 20.f;

    {
        auto entity = reg.create();
        auto& fieldSettings = reg.emplace<AcademicLifeFieldSettings>(entity);
        fieldSettings.fieldWidth = Width;
        fieldSettings.fieldHeight = Heigh;
        fieldSettings.fieldTileSize = TileSize;

        Engine::PutDefaultResource<AcademicLifeFieldSettings>(&fieldSettings);
    }

    float zPos = 1.f;

    for (int i = 0; i < Heigh; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, "EmptyWhitePixel");
            sprite.size = scale * TileSize;

            sprite.color = { 0.4f, 0.4f, 0.4f, 1 };

            if ((j == Width / 2) || (Width % 2 == 0 && (j == Width / 2 - 1)))
            {
                sprite.color = { 1,1,1,1 };
            }

            if (j == 0 || j == Width - 1)
            {
                sprite.color = { 0,0,0,1 };
            }

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f + j - static_cast<float>(Width) / 2.f) * TileSize;
            transform.position.y = (0.5f + i - static_cast<float>(Heigh) / 2.f) * TileSize;
            transform.position.z = zPos;
        }
    }

    zPos -= 0.5f;

    // player
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "Racing:police-car-bmw-z4");       // data/textures/AcademicLife
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { 2 * TileSize, 2 * TileSize * ratio };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { -TileSize * 4, -TileSize * 4, zPos };

        auto& player = reg.emplace<AcademicPlayer>(entity);
        player.horzSpeed = TileSize * 6;

        reg.emplace<ControllerMapping>(entity);

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = sprite.size;

        common_res::ParticleSpawnerSettings settings;
        settings.Setup(0.05f, {4.f, 4.f}, {-0.2f, -1.4f}, {0.2f, -0.6f}, 
                        {0.6f,0.6f,0.6f,1}, {1,1,1,1}, "EmptyWhitePixel");
        common_res::ParticleSystem::SetupParticleSystem(entity, settings);
    }

    // collisions for road bounds

    // falling entities
    int numFallingEntities = rand() % 3 + 3;
    for (int i = 0; i < numFallingEntities; i++)
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "Racing:police-car-bmw-z4");         // data/textures/AcademicLife
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { 2 * TileSize, 2 * TileSize * ratio };
        sprite.scale.y = -1;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { TileSize * (3 * (i+1) - Width/2), TileSize * (-i * 2 + Heigh/2), zPos };

        auto& falling_entity = reg.emplace<FallingEntity>(entity);
        falling_entity.speed = TileSize * (rand() % 5 + 3);

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = sprite.size;

        common_res::ParticleSpawnerSettings settings;
        settings.Setup(0.1f, {4.f, 4.f}, {-0.2f, 0.4f}, {0.2f, 1.2f}, 
                        {0.6f,0.6f,0.6f,1}, {1,1,1,1}, "EmptyWhitePixel");
        common_res::ParticleSystem::SetupParticleSystem(entity, settings);
    }
}
