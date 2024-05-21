#include "Player.h"
#include "Weapon.h"

using namespace bober_game;

Player::Player() : Character(100.0, 100.0, 10.0), m_xp(0), m_level(1)
{
	controller_ = &Engine::Instance().Registry().emplace<ControllerMapping>(instance);
	PlayerController::SetupPlayerInput(*controller_);

	movement_ = &Engine::Instance().Registry().emplace<MovementData>(instance);
	movement_->speed = m_speed;

	Engine::Registry().emplace<common::CameraFollowFocus>(instance);
}

void Player::spawn()
{
	
}

void Player::die()
{

}

void Player::collision()
{

}

void Player::shoot()
{
}

void Player::levelUp()
{
	m_level++;
}

int Player::getLevel()
{
	return m_level;
}

double Player::getSpeed() {
	return Character::getSpeed();
}

