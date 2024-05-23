#include "core/system.h"
#include "core/core.h"
#include "character_controller.h"

#include "blackboard_manager.h"

using Entity = entt::entity;
using namespace dagger;

BlackboardManager BlackboardManager::instance;

BlackboardManager& BlackboardManager::GetInstance() {
	return instance;
}


bool BlackboardManager::HasCollided()
{
	return hasCollided;
}

void BlackboardManager::SetCollided(bool colided)
{
	hasCollided = colided;
}

Entity BlackboardManager::CollidedWith()
{
	return Entity();
}

void BlackboardManager::SetCollidedWith(Entity newEnt)
{
	collidedWith = newEnt;
}
