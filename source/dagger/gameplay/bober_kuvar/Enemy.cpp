#include "Enemy.h"

Enemy::Enemy() : Character(200.0, 30.0, 10.0)
{
	lootAmount = 100.0f;
	xpDrop = 100.0f;

	movement_ = &Engine::Instance().Registry().emplace<MovementData>(instance);
	movement_->speed = m_speed;

	data_ = &Engine::Instance().Registry().emplace<EnemyData>(instance);
	data_->ID = 1;
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
