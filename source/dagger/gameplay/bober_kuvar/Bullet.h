#pragma once
#include "OurEntity.h"
#include "PlayerController.h"

using namespace bober_game;

class Bullet :
	public OurEntity
{
public:
	Bullet(float speed);
private:
	BulletSystem* bullet_system;
	float speed_;
};

