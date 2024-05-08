#pragma once
#include "gameplay/academic_life/score.h"

class Health : public Score {
public:
    static Health& Instance() {
        static Health instance;
        return instance;
    }

    void Increase(int amount) override;
    void Decrease(int amount) override;
    int GetValue() const override;

private:
    Health() : value(40) {}
    int value;
};
