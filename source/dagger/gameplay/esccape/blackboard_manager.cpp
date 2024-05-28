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

bool BlackboardManager::HasCollided(Entity entity) {
    auto it = collisionMap.find(entity);
    if(it == collisionMap.end())
        collisionMap[entity] = false;

    if (it != collisionMap.end() && it->second) {
        // Entity found in map and its value is true
        return true;
    }

    return false;
}


void BlackboardManager::SetCollided(Entity entity, bool collided) {
    collisionMap[entity] = collided;
}

Vector2 BlackboardManager::GetCollisionPosition(Entity entity) {
    if (collisionPositionMap.find(entity) != collisionPositionMap.end()) {
        return collisionPositionMap[entity];
    }
    else {
        // Return a default position or handle the absence of collision data
        return Vector2(0.0f, 0.0f);
    }
}

void BlackboardManager::SetCollisionPosition(Entity entity, const Vector2& position) {
    collisionPositionMap[entity] = position;
}

CollidedEntitiesSet BlackboardManager::GetCollidedEntities(Entity entity)
{
    auto it = collidedEntities.find(entity);
    return it != collidedEntities.end() ? it->second : CollidedEntitiesSet();
}

void BlackboardManager::RecordCollision(Entity entityA, Entity entityB)
{
    collidedEntities[entityA].insert(entityB);
    collidedEntities[entityB].insert(entityA);
}

