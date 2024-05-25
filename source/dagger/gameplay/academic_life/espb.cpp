#include "espb.h"

void ESPB::Update(int amount) {
    value += amount;
    value = std::min(value, m_max_espb);
    value = std::max(value, m_min_espb);
}

int ESPB::GetValue() const {
    return value;
}

void ESPB::Reset()
{
    value = 0;
}
