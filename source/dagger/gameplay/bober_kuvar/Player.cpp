#include "Player.h"
#include "Weapon.h"

using namespace bober_game;

Player::Player() : Character()
{
	controller_ = &Engine::Instance().Registry().emplace<ControllerMapping>(instance);
	PlayerController::SetupPlayerInput(*controller_);

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
	level_++;
}

int Player::getLevel()
{
	return level_;
}
