#include "health_system.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "gameplay/common/simple_collisions.h"


using namespace esccape;

HealthSystem::HealthSystem() : m_MaxHealth(5), m_CurrentHealth(5), m_CharacterID(0) {
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
        Engine::Dispatcher().trigger<HealthChanged>({ m_CurrentHealth, m_CharacterID });  // Emit signal when health changes
    }
}

float HealthSystem::GetCurrentHealth() const {
    return m_CurrentHealth;
}

void esccape::HealthSystem::SetCharacterID(int id)
{
    m_CharacterID = id;
}

int esccape::HealthSystem::GetCharacterID() const
{
    return m_CharacterID;
}

void HealthSystem::Heal(float amount) {
    printf("+++healing...\n\n\n");
    int newHealth = std::min(m_CurrentHealth + amount, m_MaxHealth);
    SetCurrentHealth(newHealth);
}

void HealthSystem::TakeDamage(float amount) {
    printf("---taking damageee...\n\n\n");
    float newHealth = m_CurrentHealth - amount;
    if (newHealth < 0) {
        newHealth = 0;
    }
    
    SetCurrentHealth(newHealth);
}

bool HealthSystem::IsAlive() const {
    return m_CurrentHealth > 0;
}


