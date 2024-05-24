/*#include "Player.h"

using namespace bober_game;

Player::Player() : Character()
{
	controller_ = &Engine::Instance().Registry().emplace<ControllerMapping>(instance);
	PlayerController::SetupPlayerInput(*controller_);
	AssignSprite(*sprite, "souls_like_knight_character:IDLE:idle1");
	AnimatorPlay(*animator, "souls_like_knight_character:IDLE");
	Engine::Registry().emplace<common::CameraFollowFocus>(instance);
}

void Player::spawn()
{
	//weapons.push_back(new Melee());
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
}*/
