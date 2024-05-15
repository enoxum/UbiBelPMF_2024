#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include <core/graphics/sprite.h>

using namespace dagger;

namespace academic_life
{
    void SetupWorld();

    class AcademicLife : public Game
    {
        inline String GetIniFile() override
        {
            return "academic_life.ini";
        };

        void GameplaySystemsSetup() override;
        void WorldSetup() override;
    };
}
