#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace ping_pong
{

    struct Eye
    {
        int hp {100};

        bool hasDied{false};
    };
    struct EyeTarget{
        double angle{0};
    };

    class EyeSystem
            : public System
    {
    public:

        inline String SystemName() { return "Eye System"; }

        void Run() override;
    };
}