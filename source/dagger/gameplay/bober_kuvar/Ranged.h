/*#pragma once
#include "Weapon.h"
#include "Bullet.h"

using namespace bober_game;

class Ranged :
    public Weapon
{
public:
    Ranged(int currentAmmo,int magSize,double reloadSpeed);
    void shoot(ShootEvent shoot_);
private:
    RangedWeaponSystem* ranged;
    int currentAmmo_;
    int magSize_;
    double reloadSpeed_;

    void reload();
};*/

