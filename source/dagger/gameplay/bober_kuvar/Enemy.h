#pragma once
#include "Character.h"
class Enemy :
    public Character
{
private:
    double xp_drop;
    double loot_amount;

    void spawn() override;
    void move() override;
    void die() override;
    void collision() override;
};