#include "Character.h"

Character::Character(double hp, double speed, double strength) : OurEntity(), m_hp(hp), m_speed(speed), m_strength(strength){

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

double Character::getSpeed() {
	return m_speed;
}
