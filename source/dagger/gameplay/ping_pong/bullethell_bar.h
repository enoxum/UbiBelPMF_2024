#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace ping_pong{
    struct Bar{
        Entity inner;
        Entity outer;
        float* tracking;
        float amount;
        float max;
        int width;
        int height;
        Vector3 position;
    };

    Entity createBar(float *toTrack, Vector3 position, int width, int height, Vector4 color, int max);

    class BarSystem : public System{
        inline String SystemName() { return "Bar System"; }

        void Run() override;
    };
}

