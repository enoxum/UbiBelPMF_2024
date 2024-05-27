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
    void spawn(const std::pair<int, int>&, const std::pair<int, int>&, const std::vector<std::vector<int>>&) override;

private:
    void die() override;
    void collision() override;
    void shoot();

    double xp_;
    int level_;
    ControllerMapping* controller_;
    MovementData* movement_;
};