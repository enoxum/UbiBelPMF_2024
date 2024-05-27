#pragma once
#include "Character.h"
#include "PlayerController.h"

using namespace bober_game;

class Player :
    public Character
{
public:
    Player();

    void levelUp();
    int getLevel();
    double getSpeed();

private:
    void spawn() override;
    void die() override;
    void collision() override;
    void shoot();

    double m_xp;
    int m_level;
    ControllerMapping* controller_;
    MovementData* movement_;
};