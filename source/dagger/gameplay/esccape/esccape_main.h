#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace esccape
{
    //void CreatePlayer(float tileSize_, ColorRGBA color_, Vector3 speed_, Vector3 pos_);
    void CreateMachineRandom(float playerSize, int screenWidth, int screenHeight, int zPos, int machineScale);
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