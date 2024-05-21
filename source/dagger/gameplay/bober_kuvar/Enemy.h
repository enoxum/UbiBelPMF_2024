#pragma once
#include "Character.h"
#include "PlayerController.h"

using namespace bober_game;

class Enemy :
    public Character
{
public:
    Enemy();
private:
    double xpDrop;
    double lootAmount;
    MovementData* movement_;
    EnemyData* data_;

    void spawn() override;
    void die() override;
    void collision() override;

};