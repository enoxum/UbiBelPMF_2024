#include "Enemy.h"

Enemy::Enemy() : Character(200.0, 40.0, 10.0)
{
	lootAmount = 100.0f;
	xpDrop = 100.0f;

	movement_ = &Engine::Instance().Registry().emplace<MovementData>(instance);
	movement_->speed = m_speed;

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

}
