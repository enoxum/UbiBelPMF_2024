#pragma once

#include "core/graphics/text.h"
#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace academic_life
{
    struct FallingEntity
    {
        Float32 speed;
    };

    struct FallingText
    {
        Float32 speed;
        Text text;
    };

    class FallingEntitySystem : public System
    {
    public:
        inline String SystemName() { return "Falling Entity's System"; }

        void Run() override;
    };
}
