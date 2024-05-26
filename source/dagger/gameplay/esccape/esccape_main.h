#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

#include "Worm.h"

using namespace dagger;

namespace esccape
{
    extern entt::entity healthBarEntity; 
    //void CreatePlayer(float tileSize_, ColorRGBA color_, Vector3 speed_, Vector3 pos_);
    void onHealthChanged(int newHealth);
    void CreateHealthBar(int screenWidth, int screenHeight, int zPos, int health);
    void CreateMachineRandom(int screenWidth, int screenHeight, int zPos, int machineScale);
    void CreateWorm(int zPos, int screenWidth, int screenHeight);
    void CreateNWorms(int n, int zpos, int screenWidth, int screenHeight);
    void SpawnWorm(Worm& worm, Transform& t, Sprite& sprite);
    void CreateObstacles(int zPos);
    void CreateEnemy(int zPos, int screenWidth, int screenHeight);
    void SetupWorld();


    class EsccapeGame : public Game
    {
        inline String GetIniFile() override
        {
            return "pingpong.ini";
        };

        void CoreSystemsSetup() override;
        void GameplaySystemsSetup() override;
        void WorldSetup() override;
    };
}