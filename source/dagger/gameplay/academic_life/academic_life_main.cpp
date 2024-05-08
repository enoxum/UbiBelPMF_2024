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

#include "gameplay/academic_life/health.h"
#include "gameplay/academic_life/espb.h"


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
    camera->size = { 700, 600 };
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
    constexpr int Width = 30;
    constexpr float TileSize = 20.f;

    //unsigned int ESPB = 0;

    //TO DO care about boundaries for health [-100,100]
    //int health = 40;
    Health& Health = Health::Instance();
    ESPB& ESPB = ESPB::Instance();


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

            if (ESPB.GetValue() < 80) {
                sprite.color = { 0.8f, 0.8f, 0.4f, 1 };
            }
            else if (ESPB.GetValue() < 160) {
                sprite.color = { 0.8f, 0.8f, 0.8f, 1 };
            }
            else{
                sprite.color = { 0.8f, 0.4f, 0.4f, 1 };
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
        AssignSprite(sprite, "AcademicLife:student");
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { 3 * TileSize, 3 * TileSize * ratio };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { -TileSize * 4, -TileSize * 4, zPos };

        auto& player = reg.emplace<AcademicPlayer>(entity);
        if (Health.GetValue() < -60) {
            player.horzSpeed = TileSize * 6;
        }
        else if (Health.GetValue() < -20) {
            player.horzSpeed = TileSize * 8;
        }
        else if (Health.GetValue() < 20) {
            player.horzSpeed = TileSize * 10;
        }
        else if (Health.GetValue() < 60) {
            player.horzSpeed = TileSize * 12;
        }
        else if (Health.GetValue() < 100) {
            player.horzSpeed = TileSize * 14;
        }
        else {
            player.horzSpeed = TileSize * 20;
        }

        reg.emplace<ControllerMapping>(entity);

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = sprite.size;

        common_res::ParticleSpawnerSettings settings;
        // kad implementiram levele health-a i ESPB-a resicu se svih ifova
        if (Health.GetValue() < -60) {
            settings.Setup(0.05f, { 3.f, 3.f }, { -0.2f, -1.4f }, { 0.2f, -0.6f },
                { 0,0,0,1 }, { 0.2,0.2,0.2,1 }, "EmptyWhitePixel");
        }
        else if (Health.GetValue() < -20) {
            settings.Setup(0.05f, { 2.f, 2.f }, { -0.2f, -1.4f }, { 0.2f, -0.6f },
                { 0.6f,0.6f,0.6f,1 }, { 1,1,1,1 }, "EmptyWhitePixel");
        }
        else if (Health.GetValue() < 20) {
            settings.Setup(0.0f, { 0.f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f },
                { 0,0,0,1 }, { 0,0,0,0 }, "EmptyWhitePixel");
        }
        else if (Health.GetValue() < 60) {
            settings.Setup(0.05f, { 1.5f, 1.5f }, { -0.2f, -1.4f }, { 0.2f, -0.6f },
                { 0,0.6f,0.2f,1 }, { 0,0.8,0.2f,1 }, "EmptyWhitePixel");
        }
        else if (Health.GetValue() < 100) {
            settings.Setup(0.03f, { 3.f, 3.f }, { -0.2f, -2.4f }, { 0.2f, -1.6f },
                { 0.2f,0.8f,0.2f,1 }, { 0.2f,1,0.2f,1 }, "EmptyWhitePixel");
        }
        else {
            settings.Setup(0.05f, { 2.f, 2.f }, { -0.2f, -1.4f }, { 0.2f, -0.6f },
                { 0,0.5,1,1 }, { 0,0.6,1,1 }, "EmptyWhitePixel");
        }
        common_res::ParticleSystem::SetupParticleSystem(entity, settings);
    }

    // collisions for road bounds

    // falling entities
    int numFallingEntities = rand() % 3 + 3;
    for (int i = 0; i < numFallingEntities; i++)
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "AcademicLife:cigarette");         // data/textures/AcademicLife
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { 2 * TileSize, 2 * TileSize * ratio };
        sprite.scale.y = -1;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { TileSize * (3 * (i+1) - Width/2), TileSize * (-i * 2 + Heigh/2), zPos };

        auto& falling_entity = reg.emplace<FallingEntity>(entity);
        falling_entity.speed = TileSize * (rand() % 5 + 3);

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = sprite.size;

        //TO DO ako je entity cigara - moze da ostavlja dim, 
        // a ako je jednacina - nema potrebe da ostavlja ove cestice ili moze da ostavlja neku vatru ili tako nesto
        common_res::ParticleSpawnerSettings settings;
        settings.Setup(0.1f, {4.f, 4.f}, {-0.2f, 0.4f}, {0.2f, 1.2f}, 
                        {0.6f,0.6f,0.6f,1}, {1,1,1,1}, "EmptyWhitePixel");
        common_res::ParticleSystem::SetupParticleSystem(entity, settings);
    }

}
