#pragma once
#include "core/core.h"
#include "Enemy.h"
class Room
{
private:
	int enemyCount_;
	std::list<Enemy> roomEnemies_;
	int roomType_; // za sada int

	void generateRoom();
};

