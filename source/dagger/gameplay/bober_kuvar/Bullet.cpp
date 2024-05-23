#include "Bullet.h"

Bullet::Bullet(float speed) : OurEntity(), speed_(speed)
{
	bullet_system=&Engine::Instance().Registry().emplace<BulletSystem>(instance);
	bullet_system->speed = speed;
}

Bullet::~Bullet()
{
	delete bullet_system;
}
