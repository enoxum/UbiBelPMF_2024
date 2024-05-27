#pragma once
#include "core/core.h"
#include "Enemy.h"
class Room
{
public:
	Room(int id, std::pair<int, int> top_left, std::pair<int, int> bottom_right, std::vector<std::pair<int, int>> doors_coords);
	std::pair<int, int> getTopLeft();
	std::pair<int, int> getBottomRight();
	std::vector<std::pair<int, int>> getDoorsCoords();
	int getEnemyCount();
	std::vector<Enemy*> getRoomEnemies();
	void setRoomEnemies(std::vector<Enemy*>);

private:
	int id_;
	std::pair<int, int> top_left_;
	std::pair<int, int> bottom_right_;
	std::vector<std::pair<int, int>> doors_coords_;

	int enemyCount_;
	std::vector<Enemy*> roomEnemies_;
	int roomType_; // za sada int
};

