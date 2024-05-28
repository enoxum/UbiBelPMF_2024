#pragma once

#include "core/system.h"
#include "core/core.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "BoberGame.h"
#include "PlayerController.h"

using namespace dagger;

namespace health_system
{
    struct HealthComponent
    {
        float currentHealth = 100;
        float maxHealth = 100;
    };

    struct HealthBar
    {
        float width;
        float height;
        float borderThickness;

        ColorRGBA backColor{ 0.0f, 0.0f, 0.0f, 1.0f };
        ColorRGBA fillColor{ 1.0f, 1.0f, 1.0f, 1.0f };

        float currentFill =  0.0f; // 0.0f <--> 1.0f

        Entity backgroundEntity;
        Entity fillEntity;
    };

    class Health : public System
    {
    public:
        inline String SystemName() { return "Bober Health System"; }
        void Run() override;
    };
}
