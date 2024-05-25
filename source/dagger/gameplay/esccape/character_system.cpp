#include "core/engine.h"
#include "core/core.h"
#include "character_system.h"


using namespace dagger;
using namespace esccape;

void CharacterSystem::Run()
{
    auto& reg = Engine::Registry();
    BlackboardManager& bbManager = BlackboardManager::GetInstance();

    auto characterView = reg.view<Character>();
    for (auto entity : characterView)
    {
        auto& character = characterView.get<Character>(entity);
        auto collidedPair = character.CheckCollisions();  

        Entity entityA = collidedPair.first;
        Entity entityB = collidedPair.second;

        if (entityA != entt::null && entityB != entt::null)
        {
            //printf("Collision detected between entity %d and entity %d\n", static_cast<int>(entityA), static_cast<int>(entityB));
            ResolveCollision(entityA, entityB, bbManager);
        }

    }
}           