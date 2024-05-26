#include "health_system.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "gameplay/common/simple_collisions.h"


using namespace esccape;

HealthSystem::HealthSystem() : m_MaxHealth(5), m_CurrentHealth(5) {
}

HealthSystem::~HealthSystem() {}

void HealthSystem::SetMaxHealth(float maxHealth) {
    m_MaxHealth = maxHealth;
}

float HealthSystem::GetMaxHealth() const {
    return m_MaxHealth;
}

void HealthSystem::SetCurrentHealth(float currentHealth) {
    if (m_CurrentHealth != currentHealth) {
        m_CurrentHealth = currentHealth;
        Engine::Dispatcher().trigger<HealthChanged>({ m_CurrentHealth });  // Emit signal when health changes
    }
}

float HealthSystem::GetCurrentHealth() const {
    return m_CurrentHealth;
}

void HealthSystem::Heal(float amount) {
    int newHealth = std::min(m_CurrentHealth + amount, m_MaxHealth);
    SetCurrentHealth(newHealth);
}

void HealthSystem::TakeDamage(float amount) {
    printf("taking damageee...");
    m_CurrentHealth -= amount;
    if (m_CurrentHealth < 0) {
        m_CurrentHealth = 0;
    }
    float newHealth = m_CurrentHealth;
    SetCurrentHealth(newHealth);
}

bool HealthSystem::IsAlive() const {
    return m_CurrentHealth > 0;
}


