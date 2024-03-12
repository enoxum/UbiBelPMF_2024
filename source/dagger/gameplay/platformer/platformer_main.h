#pragma once

#include "core/core.h"
#include "core/game.h"

using namespace dagger;

namespace platformer
{
    class Platformer : public Game
    {
        String GetIniFile() override
        {
            return "platformer.ini";
        }

        void GameplaySystemsSetup() override;
        void WorldSetup() override;
    };
}