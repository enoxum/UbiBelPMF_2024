#include "health_system.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "gameplay/common/simple_collisions.h"


using namespace esccape;

HealthSystem::HealthSystem() : m_MaxHealth(5), m_CurrentHealth(5) {}

HealthSystem::~HealthSystem() {}

void HealthSystem::SetMaxHealth(int maxHealth) {
    m_MaxHealth = maxHealth;
}

int HealthSystem::GetMaxHealth() const {
    return m_MaxHealth;
}

void HealthSystem::SetCurrentHealth(int currentHealth) {
    if (m_CurrentHealth != currentHealth) {
        m_CurrentHealth = currentHealth;
        Engine::Dispatcher().trigger<HealthChanged>({ m_CurrentHealth });  // Emit signal when health changes
    }
}

int HealthSystem::GetCurrentHealth() const {
    return m_CurrentHealth;
}

void HealthSystem::Heal(int amount) {
    int newHealth = std::min(m_CurrentHealth + amount, m_MaxHealth);
    SetCurrentHealth(newHealth);
}

void HealthSystem::TakeDamage(int amount) {
    int newHealth = std::max(m_CurrentHealth - amount, 0);
    SetCurrentHealth(newHealth);
}

bool HealthSystem::IsAlive() const {
    return m_CurrentHealth > 0;
}


