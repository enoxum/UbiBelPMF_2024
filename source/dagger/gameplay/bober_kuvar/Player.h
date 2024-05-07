#pragma once
#include "Character.h"
#include "PlayerController.h"

using namespace bober_game;

class Player :
    public Character
{
public:
    Player();

    void level_up();
    int get_level();
private:
    void spawn() override;
    void die() override;
    void collision() override;

    double xp;
    int level;
    ControllerMapping* controller;
};