/*#pragma once
#include "PlayerController.h"
#include "Character.h"

using namespace bober_game;

class Enemy :
    public Character
{
public:
    Enemy();
private:
    double xpDrop_;
    double lootAmount_;
    Vector3 initialPosition_;
    int numOfWaypoints_;
    MovementData* movement_;
    EnemyData* data_;
    Patrol* patrol_;

    void spawn() override;
    void die() override;
    void collision() override;
    void generatePath();
};
*/
