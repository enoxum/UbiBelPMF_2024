#include "Ranged.h"

Ranged::Ranged(int currentAmmo, int magSize, double reloadSpeed) :Weapon(5.0), currentAmmo_(currentAmmo), magSize_(magSize), reloadSpeed_(reloadSpeed)
{
	ranged = &Engine::Registry().emplace<RangedWeaponSystem>(instance);
	ranged->isMouseBtnPressed = false;

	Engine::Registry().emplace<ShootEvent>(instance);
	Engine::Dispatcher().sink<ShootEvent>().connect<&Ranged::shoot>(this);
};
 void Ranged::shoot(ShootEvent shoot_)
{	
	// Mozemo da prekidamo izvrsavanje petlje dok je currentAmmo_ = 0, i tek kada reload-uje da nastavi sa ovom funkcijom.
	if (currentAmmo_ == 0) {
		reload(); 
	}
	Bullet* bullet = new Bullet(shoot_.speed);
	//Nedostaje neka globalna lista koja ce da prati sve instance bullet-a, zasad samo sejemo zombije;
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
};

void Ranged::reload()
{
	//Neka vrsta sleep-a
	currentAmmo_ = magSize_;
	return;
};