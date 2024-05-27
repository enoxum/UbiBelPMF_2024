#include "Player.h"
#include "Weapon.h"

using namespace bober_game;

Player::Player()
	: Character(100.0, 100.0, 10.0, "souls_like_knight_character:IDLE:idle1", "souls_like_knight_character:IDLE", true, std::pair<int, int>(64, 64)), xp_(0), level_(1)
{
	controller_ = &Engine::Instance().Registry().emplace<ControllerMapping>(instance);
	PlayerController::SetupPlayerInput(*controller_);

	movement_ = &Engine::Instance().Registry().emplace<MovementData>(instance);
	movement_->speed = speed_;

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

double Player::getSpeed() {
	return Character::getSpeed();
}

