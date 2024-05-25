#pragma once

#include "core/system.h"
#include "core/core.h"
#include <string>
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "gameplay/common/simple_collisions.h"

using namespace dagger;

namespace esccape
{

    struct Worm {
        Vector3 speed{0, 0, 0};
        int startingYTop;
        int startingXRight;
        int startingYBottom;
        int startingXLeft;
        int minimumX;
        int maximumX;
        int minimumY;
        int maximumY;
        int spawnEdge;
        int scaleX;
        int scaleY;
    };

    int generateSpawnPoint(int edge, Worm& worm);
    void setPosition(int edge, int pos, Transform& t, Worm& worm, Sprite& sprite);

	class WormSystem
        : public System
    {
        public:
            inline String SystemName() { return "Worm System"; }

            void Run() override;
	};
}