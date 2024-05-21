#include "Ranged.h"

Ranged::Ranged():Weapon(5.0)
{
	ranged=&Engine::Instance().Registry().emplace<RangedWeaponSystem>(instance);
	ranged->isMouseBtnPressed = false;
}

void Ranged::shoot(Vector3 crosshairPos)
{
	
}

void Ranged::reload()
{
}