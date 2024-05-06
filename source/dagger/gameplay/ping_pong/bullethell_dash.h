#pragma once
#include "core/core.h"
#include "core/engine.h"

using namespace dagger;

namespace ping_pong{
    struct DashData{
        Float32 dashCooldown {3.0};
        Float32 dashTimer {0.0};
        bool canDash {true};
        Float32 boost{.1f};
    };

    class DashSystem : public System{
        inline String SystemName() { return "Player Dash System"; }

        void Run() override;
    };
}