#include "Ranged.h"

Ranged::Ranged(int currentAmmo, int magSize, double reloadSpeed) :Weapon(5.0), currentAmmo_(currentAmmo), magSize_(magSize), reloadSpeed_(reloadSpeed)
{
	ranged = &Engine::Registry().emplace<RangedWeaponSystem>(instance);
	ranged->isMouseBtnPressed = false;

	Engine::Registry().emplace<ShootEvent>(instance);
};

void Ranged::shoot(ShootEvent shoot_)
{
	if (currentAmmo_ == 0) {
		reload();
	}
	Bullet* bullet = new Bullet(shoot_.speed);
	//Nedostaje neka globalna lista koja ce da prati sve instance bullet-a, zasad samo sejemo zombije;
	Vector2 scale(1, 1);
	constexpr float tileSize = 20.f;
	AssignSprite(*bullet->sprite, "pizzaSlice");
	(*bullet->sprite).size = scale * tileSize;
	Engine::Registry().remove<Animator>(bullet->instance);
	currentAmmo_--;
};

void Ranged::reload()
{
	Sleep(reloadSpeed_ * 1000);
	currentAmmo_ = magSize_;
	return;
};