#pragma once
#include "Entity.h"
class Character :
    public Entity
{
private:
    double hp;
    double speed;
    double strength;

    virtual void spawn();
    virtual void move();
    virtual void die();
    virtual void collision();
};

