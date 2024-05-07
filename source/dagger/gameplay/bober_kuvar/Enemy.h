#pragma once
#include "Character.h"
class Enemy :
    public Character
{
public:
    Enemy();
private:
    double xp_drop;
    double loot_amount;

    void spawn() override;
    void die() override;
    void collision() override;
};