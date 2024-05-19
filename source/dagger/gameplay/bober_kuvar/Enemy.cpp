#include "Enemy.h"

Enemy::Enemy()
	: Character("souls_like_knight_character:IDLE:idle1", "souls_like_knight_character:IDLE", true, std::pair<int, int>(64, 64))
{
	lootAmount = 100.0f;
	xpDrop = 100.0f;
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
