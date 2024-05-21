#pragma once
#include "PlayerController.h"
#include "Character.h"

using namespace bober_game;

class Enemy :
    public Character
{
public:
    Enemy();
private:
    double xpDrop;
    double lootAmount;
    Vector3 initialPosition;
    MovementData* movement_;
    EnemyData* data_;
    Patrol* patrol_;

    void spawn() override;
    void die() override;
    void collision() override;
    void generatePath();

};