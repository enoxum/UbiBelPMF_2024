#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include <core/graphics/sprite.h>
#include <ctime>

using namespace dagger;

namespace academic_life
{
    void SetupWorld();
    void SetupMainMenu();
    void CreateBackdrop();

    class AcademicLife : public Game
    {
        inline String GetIniFile() override
        {
            return "academic_life.ini";
        };

        void GameplaySystemsSetup() override;
        void WorldSetup() override;
    };

    const float zPos = 0.5f;
    const float tileSize = 20.f;
    const int height = 30;
    const int width = 30;
}
