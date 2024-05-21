#include "Melee.h"

Melee::Melee() : Weapon(20.0)
{
	melee = &Engine::Instance().Registry().emplace<MeleeWeaponSystem>(instance);
	melee->isMouseBtnPressed = false;
}

void Melee::slash()
{
}
