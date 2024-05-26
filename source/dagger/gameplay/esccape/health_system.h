#pragma once

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

namespace esccape
{
    struct HealthChanged {
        float newHealth;
    };

    class HealthSystem {
    public:
        HealthSystem(); // Constructor
        ~HealthSystem(); // Destructor

        void SetMaxHealth(float maxHealth);
        float GetMaxHealth() const;

        void SetCurrentHealth(float currentHealth);
        float GetCurrentHealth() const;

        void Heal(float amount);
        void TakeDamage(float amount);

        bool IsAlive() const;

    private:
        float m_MaxHealth;
        float m_CurrentHealth;
    };
}

