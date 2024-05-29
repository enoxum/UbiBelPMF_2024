#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace pacman
{
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
