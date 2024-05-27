#include "gameplay/bober_kuvar/BoberGame.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/audio.h"
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
#include "core/graphics/text.h"
#include "tools/diagnostics.h"

#include "gameplay/common/parallax.h"
#include "gameplay/common/camera_focus.h"
#include "gameplay/common/simple_collisions.h"

#include "Player.h"
#include "Enemy.h"
#include "OurMap.h"
#include "PlayerController.h"

using namespace dagger;
using namespace bober_game;
using namespace common;

void BoberGame::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddSystem<PlayerController>();
    engine.AddSystem<CameraFollowSystem>();
    engine.AddSystem<SimpleCollisionsSystem>();
}

void BoberGame::SetCamera()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    //put it on 3 to simulate how the game will look
    camera->zoom = 3;
    camera->position = { 0, 0, 0 };
    camera->Update();
}

void BoberGame::WorldSetup()
{
    SetCamera();
    //Engine::GetDefaultResource<Audio>()->PlayLoop("music");
    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    int map_size = 20;
    int room_size = 4;
    OurMap* map = new OurMap(map_size, room_size);
    int n = map->get_n();
    std::vector<std::vector<int>> matrix = map->get_matrix();

    int rand_x = rand() % (n - 2) + 1;
    int rand_y = rand() % (n - 2) + 1;
    int tries = 0;
    while (matrix[rand_y][rand_x] != 0 && tries < 100) {
        rand_x = rand() % (n - 2) + 1;
        rand_y = rand() % (n - 2) + 1;
        tries++;
    }

    if (tries >= 100) {
        Logger::error("Something went really wrong! No possible spawnpoint for Bober!");
        return;
    }
    // bober
    Player* bober = new Player();
    bober->move(Vector3{ rand_x * 64, -rand_y * 64, 0.0f });

    Enemy* enemy = new Enemy();
    enemy->move(Vector3{ rand_x * 64, -rand_y * 64, 0.0f });

    OurEntity* cursor = new OurEntity("crosshair", "", false, std::make_pair(0, 0));
    //Cursor
    {
        Vector2 scale(1, 1);
        constexpr float tileSize = 10.f;
        (*cursor->sprite).size = scale * tileSize;
        reg.emplace<Cursor>(cursor->instance);
    }
    Melee* sword = new Melee();
    //Melee
    {
        Vector2 scale(1, 1);
        constexpr float tileSize = 20.f;
        //AssignSprite(*sword->sprite, "pizzaSlice");
        (*sword->sprite).size = scale * tileSize;
        reg.remove<Animator>(sword->instance);

        bober->weapons.push_back(sword);
    }

    Ranged* gun = new Ranged(8,8,2.0);
    //Ranged
    {
        Vector2 scale(1, 1);
        constexpr float tileSize = 20.f;
        //AssignSprite(*gun->sprite, "pizzaGun");
        (*gun->sprite).size = scale * tileSize;
        reg.remove<Animator>(gun->instance);

        bober->weapons.push_back(gun);
    }
    SetCamera();
}