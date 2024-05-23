#pragma once
#include "OurEntity.h"
#include "PlayerController.h"

using namespace bober_game;

class Bullet :
	public OurEntity
{
public:
	Bullet(float speed);
	~Bullet();
	BulletSystem* bullet_system;
private:
	float speed_;
};

