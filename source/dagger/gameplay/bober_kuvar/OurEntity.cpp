#include "OurEntity.h"

OurEntity::OurEntity()
{
	instance = Engine::Instance().Registry().create();

	transform = &Engine::Instance().Registry().emplace<Transform>(instance);
	(*transform).position = Vector3{ 0.0f, 0.0f, 0.0f };

	sprite = &Engine::Instance().Registry().emplace<Sprite>(instance);

	animator = &Engine::Instance().Registry().emplace<Animator>(instance);
}
