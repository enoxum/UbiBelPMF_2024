#include "Character.h"

Character::Character() : OurEntity() {

}

void Character::spawn()
{
}

void Character::move(Vector3 vector)
{
	transform->position += vector;
}

void Character::die()
{
}

void Character::collision()
{
}
