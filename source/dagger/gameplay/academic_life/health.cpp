#include "health.h"

void Health::Update(int amount) {
    value += amount;
    value = std::min(value, m_max_health);
}

int Health::GetValue() const {
    return value;
}

void Health::Reset() {
    value = 0;
}