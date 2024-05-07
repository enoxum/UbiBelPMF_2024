#pragma once
#include "Character.h"
class Enemy :
    public Character
{
public:
    Enemy();
private:
    double xpDrop;
    double lootAmount;

    void spawn() override;
    void die() override;
    void collision() override;
};