#pragma once

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

namespace esccape
{
    struct HealthChanged {
        float newHealth;
        int characterID;
    };

    class HealthSystem {
    public:
        HealthSystem();
        ~HealthSystem();

        void SetMaxHealth(float maxHealth);
        float GetMaxHealth() const;

        void SetCurrentHealth(float currentHealth);
        float GetCurrentHealth() const;

        void SetCharacterID(int id);
        int GetCharacterID() const;

        void Heal(float amount);
        void TakeDamage(float amount);

        bool IsAlive() const;

    private:
        float m_MaxHealth;
        float m_CurrentHealth;

        int m_CharacterID;
    };
}

