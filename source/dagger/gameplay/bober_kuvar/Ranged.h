#pragma once
#include "Weapon.h"
#include "Bullet.h"
class Ranged :
    public Weapon
{
    void shoot(Vector3 crosshairPos);
private:
    int currentAmmo;
    int magSize;
    double reloadSpeed;

    void reload();
};

