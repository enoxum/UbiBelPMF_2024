#include "health.h"

void Health::Increase(int amount) {
    value += amount;
    if (value > 100) value = 100;
}

void Health::Decrease(int amount) {
    value -= amount;
    if (value < 0) value = 0;
}

int Health::GetValue() const {
    return value;
}