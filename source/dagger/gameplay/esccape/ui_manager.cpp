#include "ui_manager.h"

using namespace esccape;

UIManager::UIManager(HealthSystem& healthSystem) : m_HealthSystem(healthSystem) {
    // Connect the OnHealthChanged method to the HealthChanged signal
    Engine::Dispatcher().sink<HealthChanged>().connect<&UIManager::OnHealthChanged>(this);
}

void UIManager::OnHealthChanged(const HealthChanged& event) {
    printf("Health changed to: %d\n", event.newHealth);
    // Update health bar logic:
    CreateHealthBar(800, 600, -1, event.newHealth);
}
