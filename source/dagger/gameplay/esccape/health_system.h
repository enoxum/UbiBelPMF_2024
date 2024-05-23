#pragma once

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

namespace esccape
{
    struct HealthChanged {
        int newHealth;
    };

    class HealthSystem {
    public:
        HealthSystem(); // Constructor
        ~HealthSystem(); // Destructor

        void SetMaxHealth(int maxHealth);
        int GetMaxHealth() const;

        void SetCurrentHealth(int currentHealth);
        int GetCurrentHealth() const;

        void Heal(int amount);
        void TakeDamage(int amount);

        bool IsAlive() const;

    private:
        int m_MaxHealth;
        int m_CurrentHealth;
    };
}

