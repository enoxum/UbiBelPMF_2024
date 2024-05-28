#include "academic_life_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/animations.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/graphics/gui.h"
#include "core/graphics/text.h"
#include "core/game/transforms.h"
#include "core/audio.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/common/particles.h"

#include "gameplay/academic_life/academic_life_logic.h"
#include "gameplay/academic_life/academic_player.h"
#include "gameplay/academic_life/falling_entity.h"
#include "gameplay/academic_life/score_entity.h"
#include "gameplay/academic_life/health.h"
#include "gameplay/academic_life/game_state.h"
#include "gameplay/academic_life/espb.h"

#include "gameplay/academic_life/main_menu.h"


#include "enumi.h"

#include <cmath>

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
    engine.AddSystem<ScoreEntitySystem>();
    engine.AddSystem<MainMenuInputSystem>();
    engine.AddSystem<GameStateInputSystem>();
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
    academic_life::SetupMainMenu();
    //academic_life::SetupWorld();
}

void academic_life::SetupMainMenu()
{
    Engine::GetDefaultResource<Audio>()->PlayLoop("music");

    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    constexpr Vector2 scale(1, 1);

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "AcademicLife:background");
        sprite.size = { 850.0f, 600.0f };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 0.0f, 0.0f, zPos };

        auto& menu = reg.emplace<MainMenu>(entity);

        reg.emplace<MenuControllerMapping>(entity); 
        


    }
    //start
    {
        
        auto startEntity = reg.create();
        auto& startText = reg.emplace<Text>(startEntity);
        startText.scale = Vector2{ 1.0f, 1.0f };
        startText.Set("pixel-font", "press space to start", glm::vec3(tileSize / 2.0f, -300.0f / 2.0f + tileSize / 2.0f, 0.5f), zPos);
        

    }
    //logo
    {
        auto logoEntity = reg.create();
        auto& sprite = reg.emplace<Sprite>(logoEntity);
        AssignSprite(sprite, "AcademicLife:logo");
        sprite.size = { 600.0f, 300.0f };
        sprite.position = { tileSize / 2.0f, 300.0f / 2.0f + tileSize / 2.0f, 0.5f };
 
    }


}

void academic_life::CreateBackdrop()
{
    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();
    Float32 xBorder = 850.0f;
    Float32 yBorder = 600.0f;

    //background 
    {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);

        AssignSprite(sprite, "AcademicLife:0_sky");
        sprite.size = { xBorder, yBorder };
        sprite.position = { 0,0,0.5f };
    }
    //clouds
    {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);

        AssignSprite(sprite, "AcademicLife:2_clouds");
        sprite.size = { xBorder, yBorder };
        sprite.scale = { 2, 1 };
        sprite.position = { 0,0,0.5f };
        sprite.color.a = 0.5f;
    }
    //city
    {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);

        AssignSprite(sprite, "AcademicLife:3_city");
        sprite.size = { xBorder, yBorder };
        sprite.position = { 0,0,0.5f };
    
    }
    //grass
    {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);

        AssignSprite(sprite, "AcademicLife:4_ground");
        sprite.size = { xBorder, yBorder };
        sprite.position = { 0,0,0.5f };

        
    }
    

}

void academic_life::SetupWorld()
{
    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    constexpr Vector2 scale(1, 1);

    Health& Health = Health::Instance();
    ESPB& ESPB = ESPB::Instance();

    {
        auto entity = reg.create();
        auto& fieldSettings = reg.emplace<AcademicLifeFieldSettings>(entity);
        fieldSettings.fieldWidth = width;
        fieldSettings.fieldHeight = height;
        fieldSettings.fieldTileSize = tileSize;

        Engine::PutDefaultResource<AcademicLifeFieldSettings>(&fieldSettings);
    }

    CreateBackdrop();

    // ESPB
    {
        auto entity = reg.create();
        auto& text = reg.emplace<Text>(entity);
        text.Set("pixel-font", "ESPB", glm::vec3(-700.0f / 2.0f + tileSize / 2.0f, -600.0f / 2.0f + tileSize / 2.0f, 0.5f), zPos); 

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = glm::vec3(-700.0f / 2.0f + tileSize / 2.0f, -600.0f / 2.0f + tileSize / 2.0f, 0.5f);
    }
    {
        auto entity = reg.create();
        auto& scoreText = reg.emplace<ScoreEntityBase>(entity);
        auto& text = reg.emplace<Text>(entity);

        scoreText.value = ESPB.GetValue();
        scoreText.SetText("pixel-font", std::to_string(scoreText.value), glm::vec3(-700.0f / 2.0f + tileSize / 2.0f, -600.0f / 2.0f + tileSize / 2.0f + 30.0f, 0.5f));

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = glm::vec3(-700.0f / 2.0f + tileSize / 2.0f, -600.0f / 2.0f + tileSize / 2.0f + 30.0f, 0.5f);
        reg.emplace<int>(entity, ESPB_MARKER);  
    }

    // Health
    {
        auto entity = reg.create();
        auto& text = reg.emplace<Text>(entity);
        text.Set("pixel-font", "Health", glm::vec3(700.0f / 2.0f - tileSize / 2.0f, -600.0f / 2.0f + tileSize / 2.0f, 0.5f), zPos);

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = glm::vec3(700.0f / 2.0f - tileSize / 2.0f, -600.0f / 2.0f + tileSize / 2.0f, 0.5f);
    }
    {
        auto entity = reg.create();
        auto& scoreText = reg.emplace<ScoreEntityBase>(entity);
        auto& text = reg.emplace<Text>(entity);

        scoreText.value = Health.GetValue();
        scoreText.SetText("pixel-font", std::to_string(scoreText.value), glm::vec3(700.0f / 2.0f - tileSize / 2.0f, -600.0f / 2.0f + tileSize / 2.0f + 30.0f, 0.5f));

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = glm::vec3(700.0f / 2.0f - tileSize / 2.0f, -600.0f / 2.0f + tileSize / 2.0f + 30.0f, 0.5f);
        reg.emplace<int>(entity, HEALTH_MARKER); 
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, "EmptyWhitePixel");
            sprite.size = scale * tileSize;

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
            transform.position.x = (0.5f + j - static_cast<float>(width) / 2.f) * tileSize;
            transform.position.y = (0.5f + i - static_cast<float>(height) / 2.f) * tileSize;
            transform.position.z = 1;
        }
    }

    // player
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        auto& animator = reg.emplace<Animator>(entity);
        //AssignSprite(sprite, "AcademicLife:student");
        AssignSprite(sprite, "AcademicLife:IDLE:idle1"); 
        AnimatorPlay(animator, "AcademicLife:IDLE");
       
        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { -tileSize * 4, -tileSize * 4, zPos };

        auto& player = reg.emplace<AcademicPlayer>(entity);
        player.SetSpeedBasedOnHealth(Health.GetValue(), tileSize);


        reg.emplace<ControllerMapping>(entity);

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = sprite.size;

        auto& particleSettings = reg.emplace<common_res::ParticleSpawnerSettings>(entity);
        SetParticleSettings(particleSettings, Health.GetValue());
        common_res::ParticleSystem::SetupParticleSystem(entity, particleSettings);
    }

    // falling entities
    int numFallingEntities = 5;
    for (int i = 0; i < numFallingEntities; i++)
    {
        createRandomEntity();
    }
    // timer
    {
        auto entity = reg.create();
        auto& time = reg.emplace<TimeComponent>(entity);
        time.beginTime = std::time(0);
        auto& text = reg.emplace<Text>(entity);
    }
}
