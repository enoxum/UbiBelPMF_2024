#include "Player.h"
#include "Weapon.h"

using namespace bober_game;

Player::Player() : Character()
{
	controller = &Engine::Instance().Registry().emplace<ControllerMapping>(instance);
	PlayerController::SetupPlayerInput(*controller);

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

void Player::level_up()
{
	level++;
}

int Player::get_level()
{
	return level;
}
