#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace racing_game
{
    void SetupWorld();

    class RacingGame : public Game
    {
        inline String GetIniFile() override
        {
            return "racing.ini";
        };

        void GameplaySystemsSetup() override;
        void WorldSetup() override;
    };
}
