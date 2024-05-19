#include "Player.h"
#include "Weapon.h"

using namespace bober_game;

Player::Player()
	: Character("souls_like_knight_character:IDLE:idle1", "souls_like_knight_character:IDLE", true, std::pair<int, int>(64, 64))
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
