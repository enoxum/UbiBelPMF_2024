#pragma once
#include "core/system.h"
#include "core/core.h"
#include "character_controller.h"

#include "entt/entt.hpp"

using namespace dagger;

using Entity = entt::entity;


class BlackboardManager {
public:
    static BlackboardManager& GetInstance();

    bool HasCollided();
    void SetCollided(bool colided);

    Entity CollidedWith();
    void SetCollidedWith(Entity newEnt);

private:
    static BlackboardManager instance;

    bool hasCollided = false;
    Entity collidedWith = entt::null;
};

