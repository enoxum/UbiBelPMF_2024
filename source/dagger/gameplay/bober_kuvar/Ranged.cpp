/*#include "Ranged.h"

Ranged::Ranged(int currentAmmo, int magSize, double reloadSpeed) :Weapon(5.0), currentAmmo_(currentAmmo), magSize_(magSize), reloadSpeed_(reloadSpeed)
{
	ranged = &Engine::Registry().emplace<RangedWeaponSystem>(instance);
	ranged->isMouseBtnPressed = false;

	Engine::Registry().emplace<ShootEvent>(instance);
	Engine::Dispatcher().sink<ShootEvent>().connect<&Ranged::shoot>(this);
	Engine::Dispatcher().sink<ReloadEvent>().connect<&Ranged::reload>(this);
};
 void Ranged::shoot(ShootEvent shoot_)
{	
	 if (currentAmmo_ != 0) {
		 Bullet* bullet = new Bullet(shoot_.speed);
		bullet->bullet_system->index = bullets.size();
		 bullets.push_back(bullet);
		 Vector2 scale(1, 1);
		 constexpr float tileSize = 20.f;
		 (*bullet->transform).position.x = shoot_.position.x;
		 (*bullet->transform).position.y = shoot_.position.y;
		 (*bullet->sprite).position.x = shoot_.position.x;
		 (*bullet->sprite).position.y = shoot_.position.y;
		 AssignSprite(*bullet->sprite, "pizzaSlice");
		 (*bullet->sprite).size = scale * tileSize;
		 Engine::Registry().remove<Animator>(bullet->instance);
		 currentAmmo_--;
	 }
};

void Ranged::reload()
{
	currentAmmo_ = magSize_;
	return;
};*/