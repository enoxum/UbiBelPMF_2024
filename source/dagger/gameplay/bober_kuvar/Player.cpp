/*#include "Player.h"

using namespace bober_game;

Player::Player()
	: Character(100.0, 100.0, 10.0, "BoberKuvar:beaver-SWEN", "", true, std::pair<int, int>(16, 32)), xp_(0), level_(1)
{
	controller_ = &Engine::Instance().Registry().emplace<ControllerMapping>(instance);
	PlayerController::SetupPlayerInput(*controller_);

	sprite->size *= 0.7;

	movement_ = &Engine::Instance().Registry().emplace<MovementData>(instance);
	movement_->speed = speed_;

	Engine::Registry().emplace<common::CameraFollowFocus>(instance);
}

void Player::spawn(const std::pair<int, int>& topLeft, const std::pair<int, int>& bottomRight, const std::vector<std::vector<int>>& matrix)
{
	int topLeftX = topLeft.second, topLeftY = topLeft.first;
	int	bottomRightX = bottomRight.second, bottomRightY = bottomRight.first;

	int randX = rand() % (bottomRightX - topLeftX) + topLeftX;
	int randY = rand() % (bottomRightY - topLeftY) + topLeftY;
	int tries = 0;
	while (matrix[randY][randX] != 0 && tries < 100)
	{
		randX = rand() % (bottomRightX - topLeftX) + topLeftX;
		randY = rand() % (bottomRightY - topLeftY) + topLeftY;
		tries++;
	}

	if (tries >= 100) {
		Logger::error("Something went really wrong! No possible spawnpoint for Bober!");
		return;
	}

	this->move(Vector3{ randX * 64, -randY * 64, 0.0 });
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
*/
