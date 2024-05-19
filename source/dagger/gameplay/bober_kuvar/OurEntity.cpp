#include "OurEntity.h"

OurEntity::OurEntity(const std::string& sprite_path, const std::string& animation_path, bool collidable, std::pair<int, int> collision_size)
	: sprite_path_(sprite_path), animation_path_(animation_path), collidable_(collidable), collision_size_(collision_size)
{
	instance = Engine::Instance().Registry().create();

	transform = &Engine::Instance().Registry().emplace<Transform>(instance);
	(*transform).position = Vector3{ 0.0f, 0.0f, 0.0f };

	sprite = &Engine::Instance().Registry().emplace<Sprite>(instance);
	AssignSprite(*sprite, sprite_path_);

	animator = &Engine::Instance().Registry().emplace<Animator>(instance);
	if (animation_path_ != "")
		AnimatorPlay(*animator, animation_path_);

	collision = &Engine::Instance().Registry().emplace<SimpleCollision>(instance);
	(*collision).size.x = collision_size.second;
	(*collision).size.y = collision_size.first;
}

void OurEntity::move(Vector3 vector)
{
	transform->position += vector;
}