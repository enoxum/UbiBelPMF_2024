#include "academic_life_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/graphics/gui.h"
#include "core/graphics/text.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/common/particles.h"

#include "gameplay/academic_life/academic_life_logic.h"
#include "gameplay/academic_life/academic_player.h"
#include "gameplay/academic_life/falling_entity.h"
#include "gameplay/academic_life/score_entity.h"

#include "gameplay/academic_life/health.h"
#include "gameplay/academic_life/espb.h"

#include "enumi.h"

#include <cmath>

using namespace dagger;
using namespace academic_life;

template<typename T>
std::string generate_equation(ESPB& espb, Equation& eq, T& (Equation::* get_code)() const)
{
    std::string expression = eq.to_string((eq.*get_code)());
    return eq.to_equation(expression);
}

std::string generate_equation(ESPB& espb)
{
    int espb_value = espb.GetValue();
    Equation eq = Equation::Equation(3, 4, -5, 5);

    std::string equation;
    if (espb_value < 60) equation = generate_equation(espb, eq, &Equation::get_code_simple);
    else if (espb_value < 120) equation = generate_equation(espb, eq, &Equation::get_code_medium);
    else equation = generate_equation(espb, eq, &Equation::get_code_hard);

    return equation;
}

void generate_equation_entity(entt::registry& reg, ESPB& espb, const float TileSize, const unsigned Width,
    const unsigned Height, const float zPos, int iteration)
{
    auto entity = reg.create();

    auto& transform = reg.emplace<Transform>(entity);
    transform.position = { TileSize * (3 * (iteration + 1) - Width / 2), TileSize * (-iteration * 2 + Height / 2), zPos };

    auto& falling_text = reg.emplace<FallingText>(entity);
    falling_text.speed = TileSize * (rand() % 5 + 3);
    auto& text = falling_text.text;

    std::string equation = generate_equation(espb);
    text.position = transform.position;
    text.spacing = 0.6f;
    text.message = equation;

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size = { TileSize * 2, TileSize * 2 };
}

void AcademicLife::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddSystem<AcademicPlayerInputSystem>();
    engine.AddSystem<FallingEntitySystem>();
    engine.AddSystem<AcademicLifeCollisionsLogicSystem>();
    engine.AddSystem<SimpleCollisionsSystem>();
    engine.AddSystem<common_res::ParticleSystem>();
}

constexpr int HEALTH_MARKER = 1;
constexpr int ESPB_MARKER = 2;

void AcademicLife::WorldSetup()
{
    auto& engine = Engine::Instance();

    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 850, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    academic_life::SetupWorld();
}

void SetupParticleSettingsBasedOnHealth(common_res::ParticleSpawnerSettings& settings, int health)
{
    if (health < -60) {
        settings.Setup(0.05f, { 3.f, 3.f }, { -0.2f, -1.4f }, { 0.2f, -0.6f },
            { 0,0,0,1 }, { 0.2,0.2,0.2,1 }, "EmptyWhitePixel");
    }
    else if (health < -20) {
        settings.Setup(0.05f, { 2.f, 2.f }, { -0.2f, -1.4f }, { 0.2f, -0.6f },
            { 0.6f,0.6f,0.6f,1 }, { 1,1,1,1 }, "EmptyWhitePixel");
    }
    else if (health < 20) {
        settings.Setup(0.0f, { 0.f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f },
            { 0,0,0,1 }, { 0,0,0,0 }, "EmptyWhitePixel");
    }
    else if (health < 60) {
        settings.Setup(0.05f, { 1.5f, 1.5f }, { -0.2f, -1.4f }, { 0.2f, -0.6f },
            { 0,0.6f,0.2f,1 }, { 0,0.8,0.2f,1 }, "EmptyWhitePixel");
    }
    else if (health < 100) {
        settings.Setup(0.03f, { 3.f, 3.f }, { -0.2f, -2.4f }, { 0.2f, -1.6f },
            { 0.2f,0.8f,0.2f,1 }, { 0.2f,1,0.2f,1 }, "EmptyWhitePixel");
    }
    else {
        settings.Setup(0.05f, { 2.f, 2.f }, { -0.2f, -1.4f }, { 0.2f, -0.6f },
            { 0,0.5,1,1 }, { 0,0.6,1,1 }, "EmptyWhitePixel");
    }
}

void setLifestyleEntity(int lifestyle_prob, Registry& reg, entt::entity entity, Sprite& sprite) {
    if (lifestyle_prob == 0) {
        AssignSprite(sprite, "AcademicLife:cigarette");

        reg.emplace<LifestyleChange>(entity, LifestyleChange::Cigarette);

        //ako je entity cigara - moze da ostavlja dim 
        common_res::ParticleSpawnerSettings settings;
        settings.Setup(0.1f, { 4.f, 4.f }, { -0.2f, 0.4f }, { 0.2f, 1.2f },
            { 0.6f,0.6f,0.6f,1 }, { 1,1,1,1 }, "EmptyWhitePixel");
        common_res::ParticleSystem::SetupParticleSystem(entity, settings);
    }
    else if (lifestyle_prob == 1) {
        AssignSprite(sprite, "AcademicLife:beer");
        reg.emplace<LifestyleChange>(entity, LifestyleChange::Beer);
    }
    else if (lifestyle_prob == 2) {
        AssignSprite(sprite, "AcademicLife:whey-protein");
        reg.emplace<LifestyleChange>(entity, LifestyleChange::WheyProtein);
    }
    else if (lifestyle_prob == 3) {
        AssignSprite(sprite, "AcademicLife:FishMeal");
        reg.emplace<LifestyleChange>(entity, LifestyleChange::Beer);
    }
    else {
            AssignSprite(sprite, "AcademicLife:apple");
            reg.emplace<LifestyleChange>(entity, LifestyleChange::Apple);
    }
}



void academic_life::SetupWorld()
{
    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    constexpr Vector2 scale(1, 1);

    constexpr int Height = 30;
    constexpr int Width = 30;
    constexpr float TileSize = 20.f;


    //TO DO care about boundaries for health [-100,100]
    Health& Health = Health::Instance();
    ESPB& ESPB = ESPB::Instance();

    {
        auto entity = reg.create();
        auto& fieldSettings = reg.emplace<AcademicLifeFieldSettings>(entity);
        fieldSettings.fieldWidth = Width;
        fieldSettings.fieldHeight = Height;
        fieldSettings.fieldTileSize = TileSize;

        Engine::PutDefaultResource<AcademicLifeFieldSettings>(&fieldSettings);
    }

    float zPos = 0.5f;
    // ESPB
    {
        auto entity = reg.create();
        auto& text = reg.emplace<Text>(entity);
        text.Set("pixel-font", "ESPB", glm::vec3(-700.0f / 2.0f + TileSize / 2.0f, -600.0f / 2.0f + TileSize / 2.0f, 0.5f), zPos); 

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = glm::vec3(-700.0f / 2.0f + TileSize / 2.0f, -600.0f / 2.0f + TileSize / 2.0f, 0.5f);
    }
    {
        auto entity = reg.create();
        auto& scoreText = reg.emplace<ScoreEntityBase>(entity);
        auto& text = reg.emplace<Text>(entity);

        scoreText.value = ESPB.GetValue();
        scoreText.SetText("pixel-font", std::to_string(scoreText.value), glm::vec3(-700.0f / 2.0f + TileSize / 2.0f, -600.0f / 2.0f + TileSize / 2.0f + 30.0f, 0.5f));

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = glm::vec3(-700.0f / 2.0f + TileSize / 2.0f, -600.0f / 2.0f + TileSize / 2.0f + 30.0f, 0.5f);
        reg.emplace<int>(entity, ESPB_MARKER);  //TO DO switch to EnumiScore
    }

    // Health
    {
        auto entity = reg.create();
        auto& text = reg.emplace<Text>(entity);
        text.Set("pixel-font", "Health", glm::vec3(700.0f / 2.0f - TileSize / 2.0f, -600.0f / 2.0f + TileSize / 2.0f, 0.5f), zPos);

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = glm::vec3(700.0f / 2.0f - TileSize / 2.0f, -600.0f / 2.0f + TileSize / 2.0f, 0.5f);
    }
    {
        auto entity = reg.create();
        auto& scoreText = reg.emplace<ScoreEntityBase>(entity);
        auto& text = reg.emplace<Text>(entity);

        scoreText.value = Health.GetValue();
        scoreText.SetText("pixel-font", std::to_string(scoreText.value), glm::vec3(700.0f / 2.0f - TileSize / 2.0f, -600.0f / 2.0f + TileSize / 2.0f + 30.0f, 0.5f));

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = glm::vec3(700.0f / 2.0f - TileSize / 2.0f, -600.0f / 2.0f + TileSize / 2.0f + 30.0f, 0.5f);
        reg.emplace<int>(entity, HEALTH_MARKER); //TO DO switch to EnumiScore
    }





    zPos = 1.f;

    for (int i = 0; i < Height; i++)
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
            transform.position.y = (0.5f + i - static_cast<float>(Height) / 2.f) * TileSize;
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
        player.SetSpeedBasedOnHealth(Health.GetValue(), TileSize);


        reg.emplace<ControllerMapping>(entity);

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = sprite.size;

        common_res::ParticleSpawnerSettings settings;        
        SetupParticleSettingsBasedOnHealth(settings, Health.GetValue());
        common_res::ParticleSystem::SetupParticleSystem(entity, settings);
    }

    // falling entities
    int numFallingEntities = rand() % 3 + 3;
    for (int i = 0; i < numFallingEntities; i++)
    {
        int entity_prob = rand() % 2;

        // jednacine
        if (entity_prob == 0) generate_equation_entity(reg, ESPB, TileSize, Width, Height, zPos, i);

        // lifestyle objekti
        else {
            int lifestyle_prob = rand() % 5; //da li ce da deluje pozitivno ili negativno
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            
            setLifestyleEntity(lifestyle_prob, reg, entity, sprite);

            float ratio = sprite.size.y / sprite.size.x;
            sprite.size = { 2 * TileSize, 2 * TileSize * ratio };

            auto& transform = reg.emplace<Transform>(entity);
            transform.position = { TileSize * (3 * (i + 1) - Width / 2), TileSize * (-i * 2 + Height / 2), zPos };

            auto& falling_entity = reg.emplace<FallingEntity>(entity);
            falling_entity.speed = TileSize * (rand() % 5 + 3);

            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size = sprite.size;
        }
    }
}
