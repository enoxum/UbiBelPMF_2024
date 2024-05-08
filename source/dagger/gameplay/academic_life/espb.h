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

    void Increase(int amount) override;
    void Decrease(int amount) override;
    int GetValue() const override;

private:
    ESPB() : value(0) {}
    int value;
};

