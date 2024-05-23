#pragma once

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"
#include "health_system.h"
#include "esccape_main.h"

namespace esccape {

    class UIManager {
    public:
        UIManager(HealthSystem& healthSystem);
        void OnHealthChanged(const HealthChanged& event);

    private:
        HealthSystem& m_HealthSystem;
    };
}



