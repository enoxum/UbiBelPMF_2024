#include "Enemy.h"
#include <queue>

Enemy::Enemy()
	: Character(200.0, 40.0, 10.0, "souls_like_knight_character:IDLE:idle1", "souls_like_knight_character:IDLE", true, std::pair<int, int>(16, 32))
{
	lootAmount_ = 100.0f;
	xpDrop_ = 100.0f;
	numOfWaypoints_ = (rand() % 4) + 3;

	movement_ = &Engine::Instance().Registry().emplace<MovementData>(instance);
	movement_->speed = speed_;

	data_ = &Engine::Instance().Registry().emplace<EnemyData>(instance);
	data_->focusOnPlayer = false;
	data_->visionDistance = 100.f;

	patrol_ = &Engine::Instance().Registry().emplace<Patrol>(instance);
	patrol_->currentWaypointIndex = 0;
	patrol_->forward = true;
}

void Enemy::spawn(const std::pair<int, int>& topLeft, const std::pair<int, int>& bottomRight, const std::vector<std::vector<int>>& matrix)
{
	int topLeftX = topLeft.second, topLeftY = topLeft.first;
	int	bottomRightX = bottomRight.second, bottomRightY = bottomRight.first;

	int randX = rand() % (bottomRightX - topLeftX) + topLeftX;
	int randY = rand() % (bottomRightY - topLeftY) + topLeftY;
	int tries = 0;
	while (matrix[randY][randX] != 0 && tries < 100)
	{
		randX = rand() % (bottomRightX - topLeftX) + topLeftX;
		randY = rand() % (bottomRightY - topLeftY) + topLeftY;
		tries++;
	}

	if (tries >= 100) {
		Logger::error("Something went really wrong! No possible spawnpoint for Enemy!");
		return;
	}

	initialPosition_ = Vector3{ randX * 64, -randY * 64, 0.0 };

	this->move(initialPosition_);

	generatePath(topLeft, bottomRight, matrix);
}

void Enemy::die()
{

}

void Enemy::collision()
{

}

void Enemy::generatePath(const std::pair<int, int>& topLeft, const std::pair<int, int>& bottomRight, const std::vector<std::vector<int>>& matrix)
{
	int topLeftX = topLeft.second, topLeftY = topLeft.first;
	int	bottomRightX = bottomRight.second, bottomRightY = bottomRight.first;

	patrol_->waypoints.push_back(initialPosition_);
	Vector3 waypoint = initialPosition_;
	numOfWaypoints_--;

	while (numOfWaypoints_ > 0)
	{
		int randX = rand() % (bottomRightX - topLeftX) + topLeftX;
		int randY = rand() % (bottomRightY - topLeftY) + topLeftY;
		int tries = 0;
		while (matrix[randY][randX] != 0 && tries < 100)
		{
			randX = rand() % (bottomRightX - topLeftX) + topLeftX;
			randY = rand() % (bottomRightY - topLeftY) + topLeftY;
			tries++;
		}

		if (tries >= 100) {
			Logger::error("Something went really wrong! No possible position for waypoint!");
			return;
		}
		patrol_->waypoints.push_back(Vector3{ randX * 64, -randY * 64, 0.0 });

		numOfWaypoints_--;
	}
}
