#pragma once
#include "OurEntity.h"

class Character :
    public OurEntity
{
public:
    Character();
    void move(Vector3 vector);
private:
    virtual void spawn();
    virtual void die();
    virtual void collision();

    double hp;
    double speed;
    double strength;
};