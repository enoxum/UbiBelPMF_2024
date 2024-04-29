#pragma once
#include "Character.h"
class Player :
    public Character
{
private:
    double xp;
    int level;

    void spawn() override;
    void move() override;
    void die() override;
    void collision() override;
};