#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace academic_life
{
    void SetupWorld();

    class AcademicLifeGame : public Game
    {
        inline String GetIniFile() override
        {
            return "academiclife.ini";
        };

        void CoreSystemsSetup() override;
        void GameplaySystemsSetup() override;
        void WorldSetup() override;
    };
}
