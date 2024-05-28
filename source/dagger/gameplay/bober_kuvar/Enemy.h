/*#pragma once
#include "PlayerController.h"
#include "Character.h"

using namespace bober_game;

class Enemy :
    public Character
{
public:
    Enemy();

    void spawn(const std::pair<int, int>&, const std::pair<int, int>&, const std::vector<std::vector<int>>&) override;

private:
    double xpDrop_;
    double lootAmount_;
    Vector3 initialPosition_;
    int numOfWaypoints_;
    MovementData* movement_;
    EnemyData* data_;
    Patrol* patrol_;

    void die() override;
    void collision() override;
    void generatePath(const std::pair<int, int>& topLeft, const std::pair<int, int>& bottomRight, const std::vector<std::vector<int>>& matrix);
};
*/

