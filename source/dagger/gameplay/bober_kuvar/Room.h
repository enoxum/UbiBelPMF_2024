#pragma once
#include "core/core.h"
#include "Enemy.h"
class Room
{
private:
	int enemy_count;
	std::list<Enemy> room_enemies;
	int room_type; // za sada int

	void generate_room();
};

