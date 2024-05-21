#pragma once
#include "Weapon.h"
#include "Bullet.h"

using namespace bober_game;

class Ranged :
    public Weapon
{
public:
    Ranged();
    void shoot(Vector3 crosshairPos);
private:
    RangedWeaponSystem* ranged;
    int currentAmmo;
    int magSize;
    double reloadSpeed;

    void reload();
};

