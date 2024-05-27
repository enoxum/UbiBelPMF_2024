#include "Room.h"
#include "Tile.h"

Room::Room(int id, std::pair<int, int> top_left, std::pair<int, int> bottom_right, std::vector<std::pair<int, int>> doors_coords)
{
	id_ = id;
	top_left_ = top_left;
	bottom_right_ = bottom_right;
	doors_coords_ = doors_coords;
	enemyCount_ = 2;
}

std::pair<int, int> Room::getTopLeft()
{
	return top_left_;
}

std::pair<int, int> Room::getBottomRight()
{
	return bottom_right_;
}

std::vector<std::pair<int, int>> Room::getDoorsCoords()
{
	return doors_coords_;
}

int Room::getEnemyCount()
{
	return enemyCount_;
}

std::vector<Enemy*> Room::getRoomEnemies()
{
	return roomEnemies_;
}

void Room::setRoomEnemies(std::vector<Enemy*> roomEnemies)
{
	roomEnemies_ = roomEnemies;
}
