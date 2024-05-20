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


#include <unordered_map>

#include "gameplay/common/simple_collisions.h"
#include "esccape_main.h"
#include "esccape_controller.h"
#include "player.h"

#include <random>
#include <cstdlib>

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

void esccape::CreateMachineRandom(float playerSize, int screenWidth, int screenHeight, int zPos, int machineScale)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "Esccape:masina2");
    float ratio = sprite.size.y / sprite.size.x;    
    float machineSize = playerSize * machineScale;
    printf("%f",machineSize);
    sprite.size = { machineSize, machineSize * ratio };

    auto& transform = reg.emplace<Transform>(entity);
    transform.position.x = rand() % (screenWidth - (int)machineSize) - screenWidth/2;
    transform.position.y = rand() % (screenHeight - (int)machineSize) - screenHeight/2;
    transform.position.z = zPos;

    if (transform.position.x <= playerSize && transform.position.x >= -playerSize)
        transform.position.x += 3 * playerSize;
    if (transform.position.y <= playerSize && transform.position.y >= -playerSize)
        transform.position.y += 3 * playerSize;
    

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size.x = machineSize;
    col.size.y = machineSize * ratio;
}


enum class CharacterInputCommand {
    left,
    right,
    up,
    down,
    attack,
};

struct CharacterInputContext
{
    InputContext context;
    std::unordered_map<CharacterInputCommand, int> keyMappings;

};

CharacterInputContext CreateCharacterInputContext(const InputContext& baseContext)
{
    CharacterInputContext characterContext;
    characterContext.context = baseContext;
    // Map input commands to their corresponding keys
    characterContext.keyMappings[CharacterInputCommand::left] = GLFW_KEY_A;
    characterContext.keyMappings[CharacterInputCommand::right] = GLFW_KEY_D;
    characterContext.keyMappings[CharacterInputCommand::up] = GLFW_KEY_W;
    characterContext.keyMappings[CharacterInputCommand::down] = GLFW_KEY_S;
    characterContext.keyMappings[CharacterInputCommand::attack] = GLFW_KEY_SPACE;

    return characterContext;
}

//Creating player
struct Character
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    InputReceiver& input;
    esccape::EsccapeCharacter& character;
    std::vector<CharacterInputContext> contexts;

    static Character Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& character = reg.get_or_emplace<esccape::EsccapeCharacter>(entity);

        return Character{ entity, sprite, anim, input, character };
    }

    static Character Create(
        String input_ = "",
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

        if (!input_.empty())
        {
            InputContext baseContext;
            baseContext.name = "CharacterInput";
            CharacterInputContext characterContext = CreateCharacterInputContext(baseContext);
            chr.contexts.push_back(characterContext); // Push the created context into the vector
        
        }

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

        InputContext baseContext;
        baseContext.name = "CharacterInput";
        //// Initialize baseContext...

        CharacterInputContext characterContext = CreateCharacterInputContext(InputContext());
        auto mainChar = Character::Create("CharacterInput", {1, 1, 1}, {-100, 0});
        
        
        //reg.emplace<ControllerMapping>(entity);
        //auto mainChar = Character::Create({ 1, 1, 1 }, { -100, 0 });

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

       
   */
    }
}

