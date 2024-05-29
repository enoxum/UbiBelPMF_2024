#include "gameplay/pacman/paman_main.h"

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
#include "gameplay/pacman/pacman_playerinput.h"


using namespace dagger;
using namespace pacman;

enum class CellType {
    Empty,
    Wall,
    Pellet,
    GhostHouse
};

struct Maze {
    std::vector<std::vector<CellType>> grid;
};

Maze createPacmanMaze() {
    std::vector<std::string> matrix;
    Maze maze;
    std::ifstream file("pacmanMaze.txt");
    if (!file.is_open()) {
        printf("nece fajl");
    }
    std::string line;
    while (std::getline(file, line)) {
        matrix.push_back(line);
    }
    file.close();
    maze.grid.resize(matrix.size(), std::vector<CellType>(matrix[0].size(), CellType::Empty));

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            char cell = matrix[i][j];
            if (cell == '-') {
                maze.grid[i][j] = CellType::Wall;
            }
            else if (cell == '.') {
                maze.grid[i][j] = CellType::Pellet;
            }
        }
    }
    return maze;
}

//global for now, change later for optimization
Maze g_pacmanMaze;

void pacman::PacmanGame::CoreSystemsSetup()
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

void pacman::PacmanGame::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddPausableSystem<SimpleCollisionsSystem>();
    engine.AddPausableSystem<PacmanPlayerInputSystem>();
#if defined(DAGGER_DEBUG)
 
#endif //defined(DAGGER_DEBUG)
}

void pacman::PacmanGame::WorldSetup()
{
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { screenWidth, screenHeight };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    g_pacmanMaze = createPacmanMaze();

    SetupWorld();
}
void pacman::SetupWorld()
{
    Vector2 scale(1, 1);

    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();


    constexpr int height = 22;
    constexpr int width = 24;
    constexpr float tileSize = 20.f;// / static_cast<float>(Width);

    float zPos = 1.f;

    constexpr float Space = 0.001f;
    
        
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, "EmptyWhitePixel");
            sprite.size = scale * tileSize;

            
            sprite.color.r = 0.0f;
            sprite.color.g = 0.0f;
            sprite.color.b = 0.0f;
           
            if (g_pacmanMaze.grid[i][j]==CellType::Wall)
            {
                sprite.color.r = 0.f;
                sprite.color.g = 0.011f;
                sprite.color.b = 0.439f;

                auto& col = reg.emplace<SimpleCollision>(entity);
                col.size.x = tileSize;
                col.size.y = tileSize;

            }
            else if (g_pacmanMaze.grid[i][j] == CellType::Pellet)
            {
                sprite.color.r = 0.f;
                sprite.color.g = 0.f;
                sprite.color.b = 0.f;
            }

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f + j + j * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
            transform.position.y = (0.5f + i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
            transform.position.z = zPos;
        }
    }

    zPos -= 1.f;



    // player controller setup
    const Float32 playerSize = tileSize * ((height - 2) * (1 + Space) * 0.33f);
    PacmanPlayerInputSystem::SetupPlayerBoarders(playerSize, -playerSize);
    PacmanPlayerInputSystem::s_PlayerSpeed = tileSize * 14.f;
    //1st player
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = tileSize;
        col.size.y = tileSize;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = (2.5f - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
        transform.position.y = 0;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "Pacmanart:pacmanright:1");
        sprite.size.x = tileSize;
        sprite.size.y = tileSize;

        auto& controller = reg.emplace<ControllerMapping>(entity);

        auto& mov = reg.emplace<MovementData>(entity);
        //mov.acceleration = 1000;//TODO delete acc
        mov.maxSpeed = 1;
        mov.isFrictionOn = true;//TODO delete friction

        PacmanPlayerInputSystem::SetupPlayerOneInput(controller);
    }

    auto* camera = Engine::GetDefaultResource<Camera>();
    
    
    //enemy
    for (int i = 0; i < 1; i++)
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = tileSize;
        col.size.y = tileSize;
        
        auto& enemy = reg.emplace<EnemyData>(entity);
        enemy.ID = i;

        if (i == 0) { 
            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = 200;
            transform.position.y = 170;
            transform.position.z = zPos;

            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, "Pacmanart:ghosts:blinky");
            sprite.size.x = tileSize;
            sprite.size.y = tileSize;
        }
        if (i == 1) {
            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = -200;
            transform.position.y = 170;
            transform.position.z = zPos;

            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, "Pacmanart:ghosts:blinky");
            sprite.size.x = tileSize;
            sprite.size.y = tileSize;
        }
        if (i == 2) {
            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = 200;
            transform.position.y = 170;
            transform.position.z = zPos;

            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, "Pacmanart:ghosts:blinky");
            sprite.size.x = tileSize;
            sprite.size.y = tileSize;
        }
        if (i == 3) {
            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = -200;
            transform.position.y = -170;
            transform.position.z = zPos;

            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, "Pacmanart:ghosts:blinky");
            sprite.size.x = tileSize;
            sprite.size.y = tileSize;
        }
        if (i == 4) {
            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = 200;
            transform.position.y = -170;
            transform.position.z = zPos;

            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, "Pacmanart:ghosts:blinky");
            sprite.size.x = tileSize;
            sprite.size.y = tileSize;
        }


        auto& mov = reg.emplace<MovementData>(entity);
        mov.maxSpeed = 1;
        mov.isFrictionOn = true;

        
    }

}
