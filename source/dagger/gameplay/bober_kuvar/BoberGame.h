#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace bober_game
{
    class BoberGame : public Game
    {
    private:
        int totalEnemyCount;

        inline String GetIniFile() override
        {
            return "bober_kuvar.ini";
        }

        void GameplaySystemsSetup() override;
        void WorldSetup() override;
        void SetCamera();
    };
};

