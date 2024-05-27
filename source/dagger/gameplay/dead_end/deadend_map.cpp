#include "deadend_map.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>

#include "gameplay/dead_end/deadend_obstacle.h"
#include "gameplay/dead_end/deadend_enemy.h"
#include "gameplay/dead_end/deadend_player.h"
#include "deadend_health.h"

using namespace dead_end;

void loadTiles(float zPos)
{
	auto& engine = Engine::Instance();
	auto& reg = engine.Registry();

    Vector2 scale(1, 1);
    constexpr int height = 32;
    constexpr int width = 32;
    constexpr float tileSize = 128.f;
    constexpr float Space = -0.2f;

   std::vector<std::vector<int>> tile_map{
    {8, 8, 9, 0, 15, 0, 0, 0, 2 , 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 0, 0, 0, 0, 2 , 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0 , 0, 15, 0, 9, 8, 8},
    {8, 8, 9, 0, 0, 0, 0, 15, 2 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 15, 0, 0, 0, 0, 2 , 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 0, 0, 0, 0, 2 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 15, 0, 0, 0, 2 , 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 0, 0, 0, 0, 2 , 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 0, 15, 0, 0, 2 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 15, 0, 0, 0, 0, 2 , 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 0, 0, 15, 0, 2 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 0, 0, 0, 0, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 0, 0, 15, 0, 2 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 15, 0 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 0, 0, 0, 0, 2 , 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 1, 1, 4 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 0, 15, 0, 0, 2 , 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 2 , 0, 15, 0, 9, 8, 8},
    {8, 8, 9, 0, 0, 0, 0, 15, 2 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 2 , 0, 15, 0, 9, 8, 8},
    {8, 8, 9, 15, 0, 0, 0, 0, 2 , 0, 15, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 2 , 0, 0, 0, 9, 9, 9},
    {8, 8, 9, 0, 3, 1, 1, 1, 5 , 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 , 0, 0, 0, 0, 0, 0},
    {8, 8, 9, 0, 2, 0, 15, 0, 0 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1, 1, 1, 1, 1, 1},
    {8, 8, 9, 0, 2, 0, 0, 0, 0 , 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 2 , 0, 0, 0, 0, 0, 0},
    {8, 8, 9, 0, 2, 0, 15, 0, 0 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 2 , 0, 0, 0, 9, 9, 9},
    {8, 8, 9, 0, 2, 0, 0, 0, 0 , 0, 0, 0, 0, 15, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 2, 0, 0, 0, 0 , 0, 15, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 2 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 2, 0, 15, 0, 0 , 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 15, 0, 0, 15, 0, 2 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 2, 0, 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 6, 1, 1, 1, 1 , 1, 1, 1, 1, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 0, 0, 0, 15, 0, 0 , 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 15, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 9, 8, 8},
    {8, 8, 9, 9, 9, 9, 9, 9, 9 , 9, 9, 0, 0, 2, 0, 15, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 , 9, 9, 9, 9, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8, 8 , 8, 9, 0, 0, 2, 0, 0, 9, 8, 8, 8, 8, 8, 8, 8, 8, 8 , 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8, 8 , 8, 9, 0, 0, 2, 15, 0, 9, 8, 8, 8, 8, 8, 8, 8, 8, 8 , 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8, 8 , 8, 9, 15, 0, 2, 0, 0, 9, 8, 8, 8, 8, 8, 8, 8, 8, 8 , 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8, 8 , 8, 9, 0, 0, 2, 0, 0, 9, 8, 8, 8, 8, 8, 8, 8, 8, 8 , 8, 8, 8, 8, 8, 8}
   };

   std::reverse(tile_map.begin(), tile_map.end());

     for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);

            if (i == 0 || j == 0 || i == height - 1 || j == width - 1)
            {
                auto& obstacle = reg.emplace<DeadEndObstacle>(entity);
            }

            if (tile_map[i][j] == 0)
                AssignSprite(sprite, "dead_end:Terrain:terrain_grass");

            else if(tile_map[i][j] == 1)
                AssignSprite(sprite, "dead_end:Terrain:Paths:horizontal");

            else if (tile_map[i][j] == 2)
                AssignSprite(sprite, "dead_end:Terrain:Paths:vertical");

            else if (tile_map[i][j] == 3)
                AssignSprite(sprite, "dead_end:Terrain:Paths:topLeft");

            else if (tile_map[i][j] == 4)
                AssignSprite(sprite, "dead_end:Terrain:Paths:topRight");

            else if (tile_map[i][j] == 5)
                AssignSprite(sprite, "dead_end:Terrain:Paths:bottomRight");

            else if (tile_map[i][j] == 6)
                AssignSprite(sprite, "dead_end:Terrain:Paths:bottomLeft");

            else if (tile_map[i][j] == 7)
                AssignSprite(sprite, "dead_end:Terrain:Paths:crossection1");

            else if (tile_map[i][j] == 8)
                AssignSprite(sprite, "dead_end:Terrain:Ground:bigCrops");

            else if (tile_map[i][j] == 9)
                AssignSprite(sprite, "dead_end:Terrain:Ground:smallCrops");

            else if (tile_map[i][j] == 10)
                AssignSprite(sprite, "dead_end:Terrain:Paths:crossection2");

            else if (tile_map[i][j] == 15)
                AssignSprite(sprite, "dead_end:Terrain:Ground:grass");
            

            sprite.size = scale * tileSize;
            
            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (j + j * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
            transform.position.y = (i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
            transform.position.z = zPos;
        }
    }       
}

void loadObstacles(float zPos)
{
    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    Vector2 scale(1, 1);
    constexpr int height = 32;
    constexpr int width = 32;
    constexpr float tileSize = 128.f;
    constexpr float Space = -0.2f;

    std::vector<std::vector<int>> obstacle_map{
        {0, 0, 0, 0, 0, 0, 2, 0 , 0 , 0 , 2, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2, 0 , 0 , 0 , 2, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2, 0 , 0 , 0 , 2, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2, 12, 12, 12, 2, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2, 0 , 0 , 0 , 2, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2, 0 , 0 , 0 , 2, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2, 0 , 0 , 0 , 1, 1 , 1, 1, 1 , 1, 1, 1, 1, 1, 1, 1 , 1, 1, 1, 1, 1 , 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 0 , 0 , 0 , 0, 0 , 0, 0, 0 , 0, 6, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 2, 0, 0, 0},
        {0, 0, 0, 2, 8, 0, 0, 0 , 0 , 0 , 0, 11, 0, 0, 0 , 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 13, 0, 2, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0 , 0 , 0 , 0, 10, 0, 0, 0 , 0, 0, 0, 0, 0, 0, 10, 0, 0, 8, 0, 0 , 0, 2, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0 , 0 , 0 , 0, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 3, 0 , 0, 0, 0, 0, 0 , 0, 2, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0 , 0 , 0 , 0, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 8 , 0, 2, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0 , 0 , 0 , 0, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 2, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0 , 0 , 0 , 0, 0 , 0, 0, 8 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 2, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0 , 4 , 0 , 0, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 2, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0 , 0 , 0 , 0, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 1, 1, 1, 1},
        {0, 0, 0, 2, 0, 0, 0, 0 , 0 , 0 , 0, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 8 , 0 , 8 , 0, 0 , 0, 0, 0 , 0, 8, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 4, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0 , 0 , 0 , 0, 0 , 0, 0, 0 , 0, 0, 8, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0 , 0 , 0 , 0, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 1, 1, 1, 1},
        {0, 0, 0, 2, 0, 0, 0, 0 , 0 , 0 , 0, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 2, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0 , 0 , 0 , 0, 0 , 0, 0, 0 , 0, 0, 0, 0, 8, 0, 0 , 0, 0, 0, 0, 0 , 8, 2, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0 , 0 , 0 , 0, 8 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 2, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0 , 0 , 0 , 0, 0 , 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0 , 0, 8, 0, 0, 0 , 0, 2, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0 , 0 , 0 , 0, 0 , 0, 0, 0 , 0, 0, 0, 0, 5, 0, 0 , 0, 0, 0, 0, 0 , 0, 2, 0, 0, 0},
        {0, 0, 0, 2, 8, 0, 0, 0 , 0 , 0 , 0, 0 , 0, 0, 14, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 2, 0, 0, 0},
        {0, 0, 0, 2, 1, 1, 1, 1 , 1 , 1 , 1, 1 , 0, 0, 0 , 1, 1, 1, 1, 1, 1, 1 , 1, 1, 1, 1, 1 , 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0 , 0 , 0 , 0, 2 , 0, 0, 0 , 2, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0 , 0 , 0 , 0, 2 , 0, 7, 0 , 2, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0 , 0 , 0 , 0, 2 , 0, 0, 0 , 2, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0 , 0 , 0 , 0, 2 , 0, 0, 0 , 2, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0 , 0 , 0 , 0, 2 , 0, 0, 0 , 2, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0},
    };

    std::reverse(obstacle_map.begin(), obstacle_map.end());

     for (int i = 0; i < height; i++){
        
         for (int j = 0; j < width; j++){
            
             if (obstacle_map[i][j] == 0)
                 continue;

             auto entity = reg.create();
             auto& obstacle = reg.emplace<DeadEndObstacle>(entity);
             auto& sprite = reg.emplace<Sprite>(entity);
             auto& col = reg.emplace<SimpleCollision>(entity);

             if (obstacle_map[i][j] == 1){
                 
                 AssignSprite(sprite, "dead_end:Terrain:Fences:horizontal");
                 //sprite.size = scale * tileSize;
                 sprite.size.x = 0.8*tileSize ;
                 sprite.size.y = 0.5*tileSize ;
             }

             if (obstacle_map[i][j] == 2) {
                 AssignSprite(sprite, "dead_end:Terrain:Fences:vertical2");
                 sprite.size.x = 0.3*tileSize ;
                 sprite.size.y = tileSize ;
             }
             if (obstacle_map[i][j] == 3) {
                 AssignSprite(sprite, "dead_end:Terrain:Cars:brokenHorizontal");
                 sprite.size.x = 2*tileSize;
                 sprite.size.y = tileSize;
             }
             if (obstacle_map[i][j] == 4) {
                 AssignSprite(sprite, "dead_end:Terrain:Cars:brokenWhite");
                 sprite.size.x = tileSize;
                 sprite.size.y = 2*tileSize;
             }
             if (obstacle_map[i][j] == 5) {
                 AssignSprite(sprite, "dead_end:Terrain:Cars:brown");
                 sprite.size.x = 2 * tileSize;
                 sprite.size.y = tileSize;
             }
             if (obstacle_map[i][j] == 6) {
                 AssignSprite(sprite, "dead_end:Terrain:GasStation:building");
                 sprite.size.x = 6*tileSize;
                 sprite.size.y = 4*tileSize;
             }
             if (obstacle_map[i][j] == 7) {
                 AssignSprite(sprite, "dead_end:Terrain:Cars:smokingGuy");
                 sprite.size.x = 2*tileSize;
                 sprite.size.y = tileSize;
             }
             if (obstacle_map[i][j] == 8) {
                 AssignSprite(sprite, "dead_end:Terrain:Decoration:bush");
                 sprite.size.x = 0.7*tileSize;
                 sprite.size.y = 0.7*tileSize;
             }
             if (obstacle_map[i][j] == 10) {
                 AssignSprite(sprite, "dead_end:Terrain:GasStation:bottom");
                 sprite.size.x = 0.9 * tileSize;
                 sprite.size.y = 0.9 * tileSize;
             }
             if (obstacle_map[i][j] == 11) {
                 AssignSprite(sprite, "dead_end:Terrain:GasStation:top");
                 sprite.size.x = 0.9 * tileSize;
                 sprite.size.y = 0.9 * tileSize;
             }
             if (obstacle_map[i][j] == 12) {
                 AssignSprite(sprite, "dead_end:Terrain:Decoration:youShallNotPass");
                 sprite.size.x = tileSize;
                 sprite.size.y = 0.9*tileSize;
             }
             if (obstacle_map[i][j] == 13) {
                 AssignSprite(sprite, "dead_end:Terrain:Decoration:tombstone");
                 sprite.size.x = 0.7*tileSize;
                 sprite.size.y = 0.7*tileSize;
             }
             if (obstacle_map[i][j] == 14) {
                 AssignSprite(sprite, "dead_end:Terrain:Decoration:stopSign");
                 sprite.size.x = 0.8*tileSize;
                 sprite.size.y = tileSize;
             }
             col.size.x = sprite.size.x;
             col.size.y = sprite.size.y;

             auto& transform = reg.emplace<Transform>(entity);
             transform.position.x = (j + j * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
             transform.position.y = (i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
             transform.position.z = zPos;
        }
     }
}

void loadEnemies(float zPos, float size)
{

    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    Vector2 scale(1, 1);
    constexpr int height = 32;
    constexpr int width = 32;
    constexpr float tileSize = 128.f;
    constexpr float Space = -0.2f;

    std::vector<std::vector<int>> enemy_map{
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    std::reverse(enemy_map.begin(), enemy_map.end());

    for (int i = 0; i < height; i++){
        
        for (int j = 0; j < width; j++) {

            if (enemy_map[i][j] == 0)
                continue;

            auto entity = reg.create();
            auto& enemy = reg.emplace<DeadEndEnemy>(entity);
            auto& col = reg.emplace<SimpleCollision>(entity);
            auto& sprite = reg.emplace<Sprite>(entity);


            int randomNumber = std::rand() % 2;

            std::random_device rd;

            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(30.0f, 50.0f);
            double random_speed = dis(gen);

            switch (randomNumber) {
                case 0:

                    enemy.speed = random_speed + 20.f;
                    AssignSprite(sprite, "dead_end:Enemy:zombie_1");
                    break;
                case 1:

                    enemy.speed = random_speed;
                    AssignSprite(sprite, "dead_end:Enemy:zombie_3");
                    break;
                
                default:
                    break;
            }

            sprite.size = scale * size;

            col.size.x = sprite.size.x;
            col.size.y = sprite.size.y;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (j + j * Space - static_cast<float>(width * (1 + Space)) / 2.f) * size;
            transform.position.y = (i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * size;
            transform.position.z = zPos;

            auto& health = reg.emplace<Health>(entity);
            health.currentHealth = 40.f;
            health.maxHealth = 40.f;

            

        }
     }

}