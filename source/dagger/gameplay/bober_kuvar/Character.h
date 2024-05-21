#pragma once
#include "OurEntity.h"

class Character :
    public OurEntity
{
public:
    Character(double hp, double speed, double strength);
    void move(Vector3 vector);
    
    double getSpeed();

private:
    virtual void spawn();
    virtual void die();
    virtual void collision();

protected:
    double m_hp;
    double m_speed;
    double m_strength;
};