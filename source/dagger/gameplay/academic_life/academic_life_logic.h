#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace academic_life
{
    struct AcademicLifeFieldSettings
    {
        int fieldWidth;
        int fieldHeight;
        Float32 fieldTileSize;

        Float32 GetXBoarder()
        {
            return (fieldWidth - 2) / 2 * fieldTileSize;
        }

        Float32 GetYBoarder()
        {
            return (fieldHeight) / 2 * fieldTileSize;
        }
    };

    struct AcademicLifeStats
    {
        int scores = 0;
    };

    class AcademicLifeCollisionsLogicSystem : public System
    {
        bool m_Restart = false;

    public:
        inline String SystemName() { return "AcademicLife Collisions Logic System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:
        void OnEndOfFrame();

    };
}
