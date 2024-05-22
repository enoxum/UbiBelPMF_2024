#include "Melee.h"

Melee::Melee() : Weapon(20.0)
{
	melee = &Engine::Registry().emplace<MeleeWeaponSystem>(instance);
	melee->isMouseBtnPressed = false;

	Engine::Registry().emplace<SlashEvent>(instance);
};

void Melee::slash(SlashEvent slash_)
{

};
