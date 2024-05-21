#include "espb.h"

void ESPB::Increase(int amount) {
    value += amount;
    if (value > 240) value = 240;
    if (value < 0) value = 0;
}

void ESPB::Decrease(int amount) {
    value -= amount;
    if (value < 0) value = 0;
}

int ESPB::GetValue() const {
    return value;
}