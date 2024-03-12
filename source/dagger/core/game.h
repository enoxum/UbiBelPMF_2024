#pragma once

#include "core/core.h"

namespace dagger
{
    class Engine;

    struct Game
    {
        virtual void CoreSystemsSetup();

        virtual String GetIniFile() = 0;
        virtual void GameplaySystemsSetup() = 0;
        virtual void WorldSetup() = 0;
    };
}
