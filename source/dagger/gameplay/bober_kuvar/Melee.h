#pragma once
#include "Weapon.h"

class Melee :
    public Weapon
{
    void slash();
private:
    double cooldown;
    double reach;
};

