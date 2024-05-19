#include "Character.h"

Character::Character(const std::string& sprite_path, const std::string& animation_path, bool collidable, std::pair<int, int> collision_size)
	: OurEntity(sprite_path, animation_path, collidable, collision_size)
{
	collidable_ = collidable;
	collision_size_ = collision_size;
}

void Character::spawn()
{
}

void Character::die()
{
}

void Character::collision()
{
}