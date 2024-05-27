#include "Enemy.h"
#include <queue>

Enemy::Enemy()
	: Character(200.0, 40.0, 10.0, "souls_like_knight_character:IDLE:idle1", "souls_like_knight_character:IDLE", true, std::pair<int, int>(64, 64))
{
	lootAmount_ = 100.0f;
	xpDrop_ = 100.0f;
	//initialPosition_ = ...
	numOfWaypoints_ = (rand() % 4) + 3;

	movement_ = &Engine::Instance().Registry().emplace<MovementData>(instance);
	movement_->speed = speed_;

	data_ = &Engine::Instance().Registry().emplace<EnemyData>(instance);
	data_->ID = 1;
	data_->focusOnPlayer = false;

	patrol_ = &Engine::Instance().Registry().emplace<Patrol>(instance);
	patrol_->currentWaypointIndex = 0;
	patrol_->waypoints = { {25.0f, 25.0f, 1.0f}, {25.0f, 75.0f, 1.0f}, {75.0f, 75.0f, 1.0f}, {75.0f, 25.0f, 1.0f} };
	patrol_->forward = true;
	//generatePath();
}

void Enemy::spawn()
{
	
}

void Enemy::die()
{

}

void Enemy::collision()
{

}

void Enemy::generatePath()
{
	patrol_->waypoints.push_back(initialPosition_);
	Vector3 waypoint = initialPosition_;
	numOfWaypoints_--;

	while (numOfWaypoints_ > 0)
	{
		//roomSizeX, roomSizeY
		
		


		numOfWaypoints_--;
	}

}
