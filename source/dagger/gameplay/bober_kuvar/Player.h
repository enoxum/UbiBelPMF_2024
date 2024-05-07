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
private:
    void spawn() override;
    void die() override;
    void collision() override;
    void shoot();

    double xp_;
    int level_;
    ControllerMapping* controller_;
};