#pragma once
#include "Weapon.h"
#include "Bullet.h"
class Ranged :
    public Weapon
{
private:
    int currentAmmo;
    int magSize;
    double reloadSpeed;

    void reload();
};

