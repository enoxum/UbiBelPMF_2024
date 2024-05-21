#pragma once
#include "Weapon.h"

using namespace bober_game;

class Melee :
    public Weapon
{
public:
    Melee();
    void slash();
private:
    MeleeWeaponSystem* melee;
    double cooldown;
    double reach;
};

