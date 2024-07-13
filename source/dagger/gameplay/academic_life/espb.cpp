#include "espb.h"

void ESPB::Update(int amount) {
    value += amount;
    value = std::min(value, m_max_espb);
    
    if (value >= 180)
        m_entity_speed_factor = 2.0f;
    else if (value >= 120)
        m_entity_speed_factor = 1.5f;
    else
        m_entity_speed_factor = 1.0f;
}

int ESPB::GetValue() const {
    return value;
}

void ESPB::Reset()
{
    value = 60;
    m_entity_speed_factor = 1.0f;
}
float ESPB::get_speed_factor() {
    return m_entity_speed_factor;
}
