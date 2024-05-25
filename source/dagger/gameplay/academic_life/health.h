#pragma once
#include "gameplay/academic_life/score.h"

class Health : public Score {
public:
    static Health& Instance() {
        static Health instance;
        return instance;
    }

    void Update(int amount) override;
    int GetValue() const override;
    void Reset() override;

private:
    Health() : value(50) {}
    int value;
    const int m_max_health = 100;
    const int m_min_health = 0;
};
