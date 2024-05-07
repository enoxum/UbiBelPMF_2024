#pragma once
#include "Weapon.h"
#include "Bullet.h"
class Ranged :
    public Weapon
{
private:
    double current_ammo;
    int mag_size;
    double reload_speed;

    void reload();
};

