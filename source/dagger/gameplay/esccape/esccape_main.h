#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include "health_system.h"
#include "Worm.h"

using namespace dagger;

namespace esccape
{
    extern entt::entity healthBarEntity; 
    void onHealthChanged(const HealthChanged& event);
    void CreateHealthBar(int screenWidth, int screenHeight, int chrID);
    void UpdateHealthBar(int chrID, int health);
    void CreateMachineRandom(int screenWidth, int screenHeight, int zPos, int machineScale);
    void CreateWorm(int zPos, int screenWidth, int screenHeight);
    void CreateNWorms(int n, int zpos, int screenWidth, int screenHeight);
    void SpawnWorm(Worm& worm, Transform& t, Sprite& sprite);
    void CreateObstacles(int zPos);
    void CreateEnemy(int zPos, int screenWidth, int screenHeight, Vector2 position);
    void SetupWorld();


    class EsccapeGame : public Game
    {
        inline String GetIniFile() override
        {
            return "esccape.ini";
        };

        void CoreSystemsSetup() override;
        void GameplaySystemsSetup() override;
        void WorldSetup() override;
    };
}