#include "health.h"

void Health::Update(int amount) {
    value += amount;
    value = std::min(value, m_max_health);
    value = std::max(value, m_min_health);
}

int Health::GetValue() const {
    return value;
}