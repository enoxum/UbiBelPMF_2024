/*#pragma once
#include "Character.h"
#include "PlayerController.h"
#include "Weapon.h"
#include "Ranged.h"
#include "Melee.h"

using namespace bober_game;

class Player :
    public Character
{
public:
    Player();

    void levelUp();
    int getLevel();
    std::vector<Weapon*> weapons;
    double getSpeed();
private:
    void spawn() override;
    void die() override;
    void collision() override;
    void shoot();

    double xp_;
    int level_;
    ControllerMapping* controller_;
    MovementData* movement_;
};
*/
