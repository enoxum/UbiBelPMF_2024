#pragma once
#include "core/system.h"
#include "core/core.h"
#include "character_controller.h"

#include "entt/entt.hpp"
#include <unordered_map>


using namespace dagger;

using Entity = entt::entity;


class BlackboardManager {
public:
    static BlackboardManager& GetInstance();

    bool HasCollided(Entity entity);
    void SetCollided(Entity entity, bool collided);

    Vector2 GetCollisionPosition(Entity entity);
    void SetCollisionPosition(Entity entity, const Vector2& position);

private:
    static BlackboardManager instance;

    std::unordered_map<Entity, bool> collisionMap;
    std::unordered_map<Entity, Vector2> collisionPositionMap;
};

