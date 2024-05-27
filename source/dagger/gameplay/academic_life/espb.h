#pragma once
#include "gameplay/academic_life/score.h"


class ESPB :
    public Score
{
public:
    static ESPB& Instance() {
        static ESPB instance;
        return instance;
    }

    void Update(int amount) override;
    int GetValue() const override;
    void Reset() override;
    float get_speed_factor();
    

private:
    ESPB() : value(60) {}
    int value;
    int m_max_espb = 240;
    float m_entity_speed_factor = 1.0f;
};

