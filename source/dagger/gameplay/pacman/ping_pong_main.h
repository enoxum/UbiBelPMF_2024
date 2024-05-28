#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace pacman
{
    //void CreatePingPongBall(float tileSize_, ColorRGBA color_, Vector3 speed_, Vector3 pos_);
    void SetupWorld();

    class PacmanGame : public Game
    {
        inline String GetIniFile() override
        {
            return "pacman.ini";
        };

        void CoreSystemsSetup() override;
        void GameplaySystemsSetup() override;
        void WorldSetup() override;
    };
}
