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
    engine.AddSystem<Health>();
}

void BoberGame::SetCamera()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    //put it on 3 to simulate how the game will look
    camera->zoom = 1;
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
    int room_size = 5;
    OurMap* map = new OurMap(map_size, room_size);
    int n = map->get_n();
    std::vector<std::vector<int>> matrix = map->get_matrix();

    std::vector<Room*> rooms = map->get_rooms();
    int id = 1;
    for (Room* room : rooms) {
        std::unordered_map<int, Enemy*> roomEnemies;
        int enemyCount = room->getEnemyCount();
        std::pair<int, int> topLeft = room->getTopLeft();
        std::pair<int, int> bottomRight = room->getBottomRight();

        for (size_t i = 0; i < enemyCount; i++)
        {
            Enemy* enemy = new Enemy(id++);
            enemy->spawn(topLeft, bottomRight, matrix);

            roomEnemies[enemy->data_->ID] = enemy;
        }

        room->setRoomEnemies(roomEnemies);
    }

    int randRoomIndex = rand() % rooms.size();
    std::pair<int, int> topLeft = rooms[randRoomIndex]->getTopLeft();
    std::pair<int, int> bottomRight = rooms[randRoomIndex]->getBottomRight();

    // bober
    Player* bober = new Player();
    bober->spawn(topLeft, bottomRight, matrix);

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